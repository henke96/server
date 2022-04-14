ENTRY(0)

SECTIONS {
    . = 0x100000;
    .text : {
        KEEP(*(.start));
        *(.text);
    }
    .rodata : {
        *(.rodata);
    }
    .data : ALIGN(4K) {
        *(.data);
    }
    .bss : {
        kernel_bssStart = .;
        KEEP(*(.bss));
        kernel_bssEnd = .;
    }
    kernel_stackBottom = .;
    . = . + 1M;
    . = ALIGN(16);
    kernel_stackTop = .;
    /DISCARD/ : {
        *(.comment);
    }
}