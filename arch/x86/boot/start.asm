bits 32

; Global routines
global start
global gdt_flush

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
; Argument:
;   [esp+4]: GDT pointer
gdt_flush:
    lgdt[esp+4]
    jmp 0x08:flush  ; Far jump to the Code descriptor
flush:
    mov ax, 0x10    ; Data descriptor
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ax, 0x18    ; Stack Descriptor
    mov ss, ax
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
