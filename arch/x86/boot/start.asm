bits 32

; Global routines
global start
global gdt_load

; External functions
extern kernel

section .text
    ; The Multiboot spec
    align 4
    dd 0x1badb002            ; Magic
    dd 0x03                  ; Flags
    dd - (0x1badb002 + 0x03) ; Should be zero

; Load the Global Descriptor Table into the lgdt register
; Argument:
;   [esp+4]: GDT pointer
gdt_load:
    lgdt[esp+4]     ; Tell the CPU where to find the table
    mov ax, 0x10    ; Load Data selector
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    jmp 0x08:load   ; Load Code selector and far jump
load:
    ret

; Entry point into the kernel binary
start:
    cli; Ignore Maskable Interrupts

    ; Setup the kernel stack
    mov ebp, kernel_stack
    mov esp, kernel_stack

    ; Actually start the Kernel
    push ebx; Address of Multiboot information
    push eax; Location of Multiboot bootloader magic number
    call kernel
    add esp, 8

    hlt; and catch fire

section .bss
    align 4
    resb 32768      ; Reserve 32KB for stack space
    kernel_stack:   ; "Bottom" of the stack
