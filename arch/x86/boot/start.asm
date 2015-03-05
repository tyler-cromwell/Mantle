bits 32

section .text
    ; The Multiboot spec
    align 4
    dd 0x1badb002            ; Magic
    dd 0x00                  ; Flags
    dd - (0x1badb002 + 0x00) ; Should be zero

section .data
    found db 'CPUID found', 0x0
    found_len equ $-found
    longmode db 'Long Mode available', 0x0
    longmode_len equ $-longmode
    paging db 'Paging disabled', 0x0
    paging_len equ $-paging

global start

; Kernel functions
extern console_clear
extern console_write_line
extern kernel

; Checks for CPUID
detect_cpuid:
    call console_clear
    pushfd
    pop eax
    mov ecx, eax
    xor eax, 1 << 21
    push eax
    popfd
    pushfd
    pop eax
    push ecx
    popfd
    xor eax, ecx
    jnz cpuid_found
    ret

; Prints confirmation message if CPUID is found
cpuid_found:
    push ebp
    mov ebp, esp
    push 0x0f
    push found_len
    push found
    call console_write_line
    add esp, 12
    pop ebp
    ret

; Checks if the CPU supports Long Mode
detect_long_mode:
    mov eax, 0x80000000
    cpuid
    cmp eax, 0x80000001
    jge long_mode_found
    ret

; Prints a confirmation message if Long Mode is supported
long_mode_found:
    push ebp
    mov ebp, esp
    push 0x0f
    push longmode_len
    push longmode
    call console_write_line
    add esp, 12
    pop ebp
    ret

; Entry point into the kernel binary
start:
    cli                     ; Ignore Maskable Interrupts
    call detect_cpuid
    call detect_long_mode

    mov eax, cr0
    and eax, 01111111111111111111111111111111b
    mov cr0, eax

    push ebp
    mov ebp, esp
    push 0x0f
    push paging_len
    push paging
    call console_write_line
    add esp, 12
    pop ebp

    mov esp, kernel_stack   ; Begin at the Kernel stack base
    call kernel             ; Actually start the Kernel
    hlt; and catch fire

section .bss
    align 4
    resb 32768      ; Reserve 32KB for stack space
    kernel_stack:   ; "Bottom" of the stack
