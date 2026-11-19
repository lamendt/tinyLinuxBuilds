OUTPUT_FORMAT("elf64-x86-64")
ENTRY(_start)

SECTIONS {
    . = 0x400070; /* Right after the 112-byte ELF64 header */
    .text : {
        *(.text.startup)
        *(.text*)
        *(.rodata*)
    }
    .bss : {
        *(.bss*)
        *(COMMON)
    }
}