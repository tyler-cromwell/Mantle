bits 32

; Global routines
global start
global gdt_flush

; External variables
extern gdtptr

; External functions
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

; Flush the Global Descriptor Table to the lgdt register
gdt_flush:
    lgdt[gdtptr]
    mov ax, 0x10    ; Point to the data selector
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    jmp 0x08:flush  ; Far jump to the code selector
flush:
    ret

; Entry point into the kernel binary
start:
    cli                     ; Ignore Maskable Interrupts
    call console_clear

    mov ebp, kernel_stack   ; Setup the kernel stack
    mov esp, kernel_stack

    call kernel             ; Actually start the Kernel
    hlt; and catch fire

section .bss
    align 4
    resb 32768      ; Reserve 32KB for stack space
    kernel_stack:   ; "Bottom" of the stack
