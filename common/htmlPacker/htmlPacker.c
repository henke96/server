// HTML packer that allows includes and outputs the result as a C array.
// Includes are done with <!--INCLUDE(src/main.js)--> or with /*INCLUDE(src/main.js)*/

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <limits.h>
#include <string.h>

static char *buffer = NULL;
static size_t bufferLength = 0;

#define READ_SIZE 4096
static int replaceWithFile(size_t replaceIndex, size_t replaceLength, char *fileName, size_t fileNameLength) {
    char *fileNameZ = malloc(fileNameLength + 1);
    if (fileNameZ == NULL) return -1;
    memcpy(fileNameZ, fileName, fileNameLength);
    fileNameZ[fileNameLength] = '\0';

    int status;
    FILE *handle = fopen(fileNameZ, "r");
    if (handle == NULL) {
        status = -2;
        goto cleanup_fileNameZ;
    }

    char *content = NULL;
    size_t contentLength = 0;

    for (;;) {
        content = realloc(content, contentLength + READ_SIZE);
        if (content == NULL) {
            status = -3;
            goto cleanup_content;
        }
        size_t readSize = fread(&content[contentLength], 1, READ_SIZE, handle);
        contentLength += readSize;
        if (readSize != READ_SIZE) {
            if (feof(handle) != 0) break;
            status = -4;
            goto cleanup_content;
        }
    }

    size_t newBufferLength = bufferLength + (contentLength - replaceLength);
    if (newBufferLength > bufferLength) {
        char *newBuffer = realloc(buffer, newBufferLength + 1);
        if (newBuffer == NULL) {
            status = -5;
            goto cleanup_handle;
        }
        buffer = newBuffer;
    }
    // Move existing content
    memmove(&buffer[replaceIndex + contentLength], &buffer[replaceIndex + replaceLength], bufferLength - (replaceLength + replaceIndex));
    bufferLength = newBufferLength;
    buffer[bufferLength] = '\0';
    // Insert new content
    memcpy(&buffer[replaceIndex], &content[0], contentLength);

    status = 0;
    cleanup_content:
    free(content);
    cleanup_handle:
    fclose(handle);
    cleanup_fileNameZ:
    free(fileNameZ);
    return status;
}
#undef READ_SIZE

static int writeToFile(char *fileName, char *content, size_t contentLength) {
    FILE *handle = fopen(fileName, "w");
    if (handle == NULL) return -1;

    int status;
    if (fwrite(content, 1, contentLength, handle) != contentLength) {
        status = -2;
        goto cleanup_handle;
    };
    status = 0;
    cleanup_handle:
    fclose(handle);
    return status;
}

static int addHttpHeader(void) {
    size_t digits = 1;
    size_t magnitude = 10;
    while (bufferLength >= magnitude) {
        ++digits;
        magnitude *= 10;
    }
    char responseHttpStart[] = "HTTP/1.1 200 OK\r\nContent-Length:";
    char responseHttpEnd[] = "\r\n\r\n";
    size_t httpLength = (sizeof(responseHttpStart) - 1) + digits + (sizeof(responseHttpEnd) - 1);
    size_t newBufferLength = httpLength + bufferLength;

    char *newBuffer = realloc(buffer, newBufferLength + 1);
    if (newBuffer == NULL) return -1;

    size_t contentLength = bufferLength;
    buffer = newBuffer;
    bufferLength = newBufferLength;
    memmove(&buffer[httpLength], &buffer[0], contentLength);
    buffer[bufferLength] = '\0';

    memcpy(&buffer[0], responseHttpStart, sizeof(responseHttpStart) - 1);

    char *bufferPos = &buffer[sizeof(responseHttpStart) - 1];
    while (magnitude >= 10) {
        magnitude /= 10;
        size_t digitValue = contentLength / magnitude;
        contentLength -= digitValue * magnitude;
        *bufferPos = (char)('0' + digitValue);
        ++bufferPos;
    }

    memcpy(bufferPos, responseHttpEnd, sizeof(responseHttpEnd) - 1);
    bufferPos += (sizeof(responseHttpEnd) - 1);
    return 0;
}

static int writeHeaderOutput(char *fileName, char *arrayName) {
    char start[] = "static uint8_t ";
    char afterName[] = "[] = {";
    char betweenBytes[] = ",";
    char end[] = "};\n";

    size_t maxLength = (
        (sizeof(start) - 1) +
        strlen(arrayName) +
        (sizeof(afterName) - 1) +
        (3 + (sizeof(betweenBytes) - 1)) * (bufferLength - 1) +
        (sizeof(end) - 1) +
        1
    );

    char *outBuffer = malloc(maxLength);
    if (outBuffer == NULL) return -1;
    outBuffer[0] = '\0';
    strcat(outBuffer, start);
    strcat(outBuffer, arrayName);
    strcat(outBuffer, afterName);

    for (size_t i = 0; i < bufferLength; ++i) {
        sprintf(&outBuffer[strlen(outBuffer)], "%" PRIu8, (uint8_t)buffer[i]);
        if (i != bufferLength - 1) strcat(outBuffer, betweenBytes);
    }
    strcat(outBuffer, end);

    int status = writeToFile(fileName, outBuffer, strlen(outBuffer));
    if (status < 0) {
        status = -2;
        goto cleanup_outBuffer;
    }

    status = 0;
    cleanup_outBuffer:
    free(outBuffer);
    return status;
}

static int handleInclude(char *includeStartPattern, char *includeEndPattern) {
    char *includeStart = strstr(buffer, includeStartPattern);
    if (includeStart == NULL) return 1;
    char *includeEnd = strstr(includeStart, includeEndPattern);
    if (includeEnd == NULL) {
        printf("Error: Unclosed include\n");
        return -1;
    }
    char *name = &includeStart[strlen(includeStartPattern)];
    size_t nameLength = (size_t)(includeEnd - name);
    int status = replaceWithFile((size_t)(includeStart - buffer), (size_t)(includeEnd + strlen(includeEndPattern) - includeStart), name, nameLength);
    if (status < 0) {
        if (nameLength <= INT_MAX) printf("Error: Failed to include file %.*s (%d)\n", (int)nameLength, name, status);
        return -2;
    }
    return 0;
}

int main(int argc, char **argv) {
    if (argc != 3) {
        printf("Usage: %s infile.html outname\n", argv[0]);
        return 1;
    }
    int status = replaceWithFile(0, 0, argv[1], strlen(argv[1]));
    if (status < 0) {
        printf("Error: Failed to load initial file %s (%d)\n", argv[1], status);
        return 1;
    }

    int complete = 0;
    while (!complete) {
        complete = 1;
        status = handleInclude("<!--INCLUDE(", ")-->");
        if (status < 0) {
            printf("Error: Failed to handle Html include (%d)\n", status);
            status = 1;
            goto cleanup_buffer;
        }
        complete &= status;
        status = handleInclude("/*INCLUDE(", ")*/");
        if (status < 0) {
            printf("Error: Failed to handle Javascript/Css include (%d)\n", status);
            status = 1;
            goto cleanup_buffer;
        }
        complete &= status;
    }

    size_t outNameLength = strlen(argv[2]);
    char *outName = malloc(outNameLength + 6); // 6 enough for ".html" and ".h".
    if (outName == NULL) {
        printf("Error: Failed to allocate memory\n");
        status = 1;
        goto cleanup_buffer;
    }
    memcpy(outName, argv[2], outNameLength);
    memcpy(&outName[outNameLength], ".html", 6);
    status = writeToFile(outName, buffer, bufferLength);
    if (status < 0) {
        printf("Error: Failed to write html output (%d)\n", status);
        status = 1;
        goto cleanup_outName;
    }

    status = addHttpHeader();
    if (status < 0) {
        printf("Error: Failed to add HTTP header (%d)\n", status);
        status = 1;
        goto cleanup_outName;
    }

    memcpy(&outName[outNameLength], ".h", 3);
    status = writeHeaderOutput(outName, argv[2]);
    if (status < 0) {
        printf("Error: Failed to write header output (%d)\n", status);
        status = 1;
        goto cleanup_outName;
    }
    status = 0;
    cleanup_outName:
    free(outName);
    cleanup_buffer:
    free(buffer);
    return status;
}
