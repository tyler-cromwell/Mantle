bits 32

section .text
    ; The Multiboot spec
    align 4
    dd 0x1badb002            ; Magic
    dd 0x00                  ; Flags
    dd - (0x1badb002 + 0x00) ; Should be zero

section .data
    file        db '[', _FILE_, ']: ', 0x0
    file_len    equ $-file

; Kernel binary entry point
global start
; Kernel functions
extern console_clear
extern detect_long_mode
extern paging_disabled
extern kernel

; Entry point into the kernel binary
start:
    cli                     ; Ignore Maskable Interrupts
    call console_clear
    call detect_long_mode

    ; Disable 32-bit Protected mode Paging
    mov eax, cr0
    and eax, 0x7fffffff
    mov cr0, eax
    call paging_disabled

    mov esp, kernel_stack   ; Begin at the Kernel stack base
    call kernel             ; Actually start the Kernel

    hlt; and catch fire

section .bss
    align 4
    resb 32768      ; Reserve 32KB for stack space
    kernel_stack:   ; "Bottom" of the stack
