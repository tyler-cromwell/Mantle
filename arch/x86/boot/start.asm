bits 32

; Global routines
global start
global gdt_load

; External functions
extern console_clear
extern kernel

section .text
    ; The Multiboot spec
    align 4
    dd 0x1badb002            ; Magic
    dd 0x00                  ; Flags
    dd - (0x1badb002 + 0x00) ; Should be zero

; Load the Global Descriptor Table into the lgdt register
; Argument:
;   [esp+4]: GDT pointer
gdt_load:
    lgdt[esp+4]
    jmp 0x08:load  ; Far jump to the Code descriptor
load:
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
    cli; Ignore Maskable Interrupts

    ; Setup the kernel stack
    mov ebp, kernel_stack
    mov esp, kernel_stack

    ; Actually start the Kernel
    push ebx; Address of Multiboot information
    call kernel
    add esp, 4

    hlt; and catch fire

section .bss
    align 4
    resb 32768      ; Reserve 32KB for stack space
    kernel_stack:   ; "Bottom" of the stack
