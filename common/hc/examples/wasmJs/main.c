#include "../../src/hc.h"

hc_WASM_IMPORT("env", "print")
void print(int32_t number);

hc_WASM_EXPORT("addAndPrint")
int32_t addAndPrint(int32_t a, int32_t b) {
    int32_t result = a + b;
    print(result);
    return result;
}
