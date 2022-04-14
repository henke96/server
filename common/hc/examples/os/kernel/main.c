#include "../../../src/hc.h"
#include "../../../src/efi.h"
#include "../../../src/libc/musl.c"

extern uint8_t kernel_bssStart;
extern uint8_t kernel_bssEnd;
extern uint8_t kernel_stackTop;
extern uint8_t kernel_stackBottom;

// Entry point.
asm(
    ".section .start,\"ax\",@progbits\n"
    "start:\n"
    // Clear direction and interrupt flags.
    "cld\n"
    "cli\n"
    // Clear segment registers.
    "xor %eax, %eax\n"
    "mov %eax, %ds\n"
    "mov %eax, %es\n"
    "mov %eax, %ss\n"
    "mov %eax, %fs\n"
    "mov %eax, %gs\n"
    // Clear bss segment.
    "mov %rdi, %r8\n"
    "lea kernel_bssStart(%rip), %rdi\n"
    "lea kernel_bssEnd(%rip), %rcx\n"
    "sub %rdi, %rcx\n"
    "shr $3, %rcx\n"
    "rep stosq\n"
    "mov %r8, %rdi\n"
    // Setup stack.
    "lea kernel_stackTop(%rip), %esp\n"
    // Call the main function.
    "call main\n"
    "hlt\n"
);

void main(struct efi_graphicsOutputProtocol *graphics) {
    // Do some drawing.
    uint32_t red = 0;
    uint32_t green = 0;
    uint32_t blue = 0;
    int32_t numPixels = (int32_t)(graphics->mode->info->verticalResolution * graphics->mode->info->horizontalResolution);
    uint32_t *frameBuffer = (uint32_t *)graphics->mode->frameBufferBase;
    for (;;) {
        uint32_t colour = (red << 16) | (green << 8) | blue;
        for (int32_t i = 0; i < numPixels; ++i) frameBuffer[i] = colour;

        // Continuous iteration of colours.
        if (red == 0 && green == 0 && blue != 255) ++blue;
        else if (red == 0 && green != 255 && blue == 255) ++green;
        else if (red == 0 && green == 255 && blue != 0) --blue;
        else if (red != 255 && green == 255 && blue == 0) ++red;
        else if (red == 255 && green == 255 && blue != 255) ++blue;
        else if (red == 255 && green != 0 && blue == 255) --green;
        else if (red == 255 && green == 0 && blue != 0) --blue;
        else --red;
    }
}
