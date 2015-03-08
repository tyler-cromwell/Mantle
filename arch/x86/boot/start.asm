bits 32

section .text
    ; The Multiboot spec
    align 4
    dd 0x1badb002            ; Magic
    dd 0x00                  ; Flags
    dd - (0x1badb002 + 0x00) ; Should be zero

section .data
    file        db '[', _FILE_, ']: ', 0x0
    found       db 'CPUID found', 0x0

    file_len        equ $-file
    found_len       equ $-found

; Kernel binary entry point
global start
; Kernel functions
extern console_clear
extern console_write
extern console_write_line
extern detect_long_mode
extern disable_paging
extern kernel

; Checks for CPUID
detect_cpuid:
    pushfd
    pop eax
    mov ecx, eax
    xor eax, 0x200000
    push eax
    popfd
    pushfd
    pop eax
    push ecx
    popfd
    xor eax, ecx
    jnz cpuid_found
    jz stop
    ret

; Prints confirmation message if CPUID is found
cpuid_found:
    push ebp
    mov ebp, esp

    push 0x07
    push file_len
    push file
    call console_write
    add esp, 12

    push 0x0e
    push found_len
    push found
    call console_write_line
    add esp, 12

    pop ebp
    ret

; Entry point into the kernel binary
start:
    cli                     ; Ignore Maskable Interrupts
    call console_clear

    call detect_cpuid
    call detect_long_mode
    call disable_paging

    mov esp, kernel_stack   ; Begin at the Kernel stack base
    call kernel             ; Actually start the Kernel

    hlt; and catch fire

stop:
    hlt

section .bss
    align 4
    resb 32768      ; Reserve 32KB for stack space
    kernel_stack:   ; "Bottom" of the stack
