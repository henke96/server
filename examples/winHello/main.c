#include "../../src/hc.h"
#include "../../src/util.c"
#include "../../src/windows/windows.h"

void noreturn main(void) {
    void *stdOutHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    if (stdOutHandle == INVALID_HANDLE_VALUE) ExitProcess(1);
    WriteFile(stdOutHandle, "Hello!\n", 7, NULL, NULL);
    MessageBoxA(NULL, "Hey!", "Yo", MB_OK);
    ExitProcess(0);
}