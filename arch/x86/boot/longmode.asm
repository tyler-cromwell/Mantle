global detect_long_mode
global disable_paging
extern console_write

section .data
    file        db '[', _FILE_, ']: ', 0x0
    failed      db 'Cannot switch into Long mode. Halting...', 0x0
    success     db 'Long mode is supported', 0x0a, 0x0

    file_len        equ $-file
    failed_len      equ $-failed
    success_len     equ $-success

section .text

detect_long_mode:
    mov eax, 0x80000000
    cpuid
    cmp eax, 0x80000001
    jb cannot_switch        ; Extended CPUID functions not found
    
    mov eax, 0x80000001
    cpuid
    test edx, 1 << 29
    jz cannot_switch        ; Long mode unavailable

    push ebp
    mov ebp, esp

    push 0x07
    push file_len
    push file
    call console_write
    add esp, 12

    push 0x0e
    push success_len
    push success
    call console_write
    add esp, 12

    pop ebp
    ret

disable_paging:
    mov eax, cr0
    and eax, 0x7fffffff
    mov cr0, eax
    ret
    
cannot_switch:
    push ebp
    mov ebp, esp

    push 0x07
    push file_len
    push file
    call console_write
    add esp, 12

    push 0x04
    push failed_len
    push failed
    call console_write
    add esp, 12

    pop ebp
    hlt                 ; Because we can't switch to Long mode, we must stop
