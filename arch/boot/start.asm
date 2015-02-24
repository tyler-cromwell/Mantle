bits 32
section .text
    ; The Multiboot spec
    align 4
    dd 0x1BADB002            ; Magic
    dd 0x00                  ; Flags
    dd - (0x1BADB002 + 0x00) ; Should be zero

global start
extern kernel

start:
    cli         ; Clear Interrupts
    call kernel ; Actually start the Kernel
    hlt; and catch fire

section .bss
resb 32768      ; Reserve 32KB for stack space
stack_space:
