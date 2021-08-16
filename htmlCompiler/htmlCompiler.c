// HTML compiler that allows includes and outputs the result as a C array.
// Includes are done with <!--INCLUDE(src/main.js)--> or with /*INCLUDE(src/main.js)*/

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>

static char *buffer = NULL;
static int32_t bufferLength = 0;

static int replaceWithFile(int32_t replaceIndex, int32_t replaceLength, char *fileName, int32_t fileNameLength) {
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

    if (fseek(handle, 0, SEEK_END) != 0) {
        status = -3;
        goto cleanup_handle;
    }
    int32_t length = ftell(handle);
    if (length < 0) {
        status = -4;
        goto cleanup_handle;
    }
    rewind(handle);

    int32_t newBufferLength = bufferLength + (length - replaceLength);
    if (newBufferLength > bufferLength) {
        char *newBuffer = realloc(buffer, newBufferLength + 1);
        if (newBuffer == NULL) {
            status = -5;
            goto cleanup_handle;
        }
        buffer = newBuffer;
    }
    memmove(&buffer[replaceIndex + length], &buffer[replaceIndex + replaceLength], bufferLength - (replaceLength + replaceIndex));
    buffer[newBufferLength] = '\0';
    bufferLength = newBufferLength;
    int readSize = fread(&buffer[replaceIndex], 1, length, handle);
    if (readSize != length) {
        status = -6;
        goto cleanup_handle;
    }
    status = 0;
    cleanup_handle:
    fclose(handle);
    cleanup_fileNameZ:
    free(fileNameZ);
    return status;
}

static int writeToFile(char *fileName, char *content, int32_t contentLength) {
    FILE *handle = fopen(fileName, "w");
    if (handle == NULL) return -1;

    int status;
    if (fwrite(content, 1, contentLength, handle) != (size_t)contentLength) {
        status = -2;
        goto cleanup_handle;
    };
    status = 0;
    cleanup_handle:
    fclose(handle);
    return status;
}

static int writeHeaderOutput(char *fileName, char *arrayName) {
    char start[] = "#include <stdint.h>\nstatic uint8_t ";
    char afterName[] = "[] = {";
    char betweenBytes[] = ",";
    char end[] = "};\n";

    int32_t maxLength = (
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

    for (int32_t i = 0; i < bufferLength; ++i) {
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
    int32_t nameLength = (int32_t)(includeEnd - name);
    int status = replaceWithFile((int32_t)(includeStart - buffer), (int32_t)(includeEnd + strlen(includeEndPattern) - includeStart), name, nameLength);
    if (status < 0) {
        printf("Error: Failed to include file %.*s (%d)\n", nameLength, name, status);
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

    int32_t outNameLength = strlen(argv[2]);
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