#include "../../src/hc.h"
#include "../../src/efi.h"

int64_t main(hc_UNUSED void *imageHandle, struct efi_systemTable *systemTable) {
    int64_t status = systemTable->consoleOut->outputString(systemTable->consoleOut, u"Hello World\r\n");
    if (status < 0) return status;
    return 0;
}