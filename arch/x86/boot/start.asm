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
    cli                     ; Ignore Maskable Interrupts
    mov esp, kernel_stack   ; Begin at the Kernel stack base
    call kernel             ; Actually start the Kernel
    hlt; and catch fire

section .bss
    align 4
    resb 32768  ; Reserve 32KB for stack space
    kernel_stack:
