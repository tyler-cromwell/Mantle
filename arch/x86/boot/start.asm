bits 32

; Kernel binary entry point
global start
; Kernel functions
extern console_clear
extern kernel

section .data
    file        db '[', _FILE_, ']: ', 0x0
    file_len    equ $-file

section .text
    ; The Multiboot spec
    align 4
    dd 0x1badb002            ; Magic
    dd 0x00                  ; Flags
    dd - (0x1badb002 + 0x00) ; Should be zero

; Entry point into the kernel binary
start:
    cli                     ; Ignore Maskable Interrupts
    call console_clear
    mov esp, kernel_stack   ; Begin at the Kernel stack base
    call kernel             ; Actually start the Kernel
    hlt; and catch fire

section .bss
    align 4
    resb 32768      ; Reserve 32KB for stack space
    kernel_stack:   ; "Bottom" of the stack
