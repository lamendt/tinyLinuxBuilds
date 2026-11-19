BITS 64
%define VADDR 0x400000
ORG VADDR

ehdr:
    db 0x7F, "ELF"          ; 0x00: magic
    db 2, 1, 1, 0           ; 0x04: 64-bit, little-endian, v1
    dq 0                    ; 0x08: padding
    dw 2                    ; 0x10: e_type = ET_EXEC
    dw 0x3E                 ; 0x12: e_machine = EM_X86_64
    dd 1                    ; 0x14: e_version = 1
    dq _start               ; 0x18: e_entry
    dq phdr - ehdr          ; 0x20: e_phoff = 56 (0x38)
    dq 0                    ; 0x28: e_shoff = 0
    dd 0                    ; 0x30: e_flags = 0
    dw 64                   ; 0x34: e_ehsize = 64
    dw 56                   ; 0x36: e_phentsize = 56

phdr:
    dd 1                    ; 0x38: p_type = PT_LOAD   (also e_phnum = 1 at 0x38)
    dd 7                    ; 0x3C: p_flags = R|W|X    (also e_shentsize/shnum/shstrndx)
    dq 0                    ; 0x40: p_offset = 0
    dq VADDR                ; 0x48: p_vaddr = 0x400000
    dq VADDR                ; 0x50: p_paddr = 0x400000
    dq filesize             ; 0x58: p_filesz
    dq filesize + 65536     ; 0x60: p_memsz (+64KB .bss)
    dq 0x1000               ; 0x68: p_align

_start:
    incbin "make/code.bin"       ; Starts cleanly at byte 112 (0x70)

filesize equ $ - ehdr
