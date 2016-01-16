;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;  Copyright (C) 2015 Tyler Cromwell <tyler@csh.rit.edu>
;
;  This file is part of Ritchie.
;
;  Ritchie is free software: you can redistribute it and/or modify
;  it under Version 2 of the terms of the GNU General Public License
;  as published by the Free Software Foundation.
;
;  Ritchie is distributed in the hope that it will be useful,
;  but WITHOUT ANY WARRANTY; without even the implied warranty of
;  MERCHANTABILITY of FITNESS FOR A PARTICULAR PURPOSE. See the
;  GNU General Public License for more details.
;
;  You should have received a copy of the GNU General Public License
;  along with Ritchie.
;  If not, see <http://www.gnu.org/licenses/old-licenses/gpl-2.0.html>
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

[bits 32]

; Global Symbols
global kernel_boot

; External Symbols
extern init_kernel

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;  Multiboot Header
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
[section .multiboot]
    align 4
    dd 0x1badb002                   ; Magic number
    dd 0x00000003                   ; Flags
    dd -(0x1badb002 + 0x00000003)   ; Checksum


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; BSS Section
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
[section .bss]
    align 16
    stack_bottom:
    resb 32768
    stack_top:


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Data Section
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
[section .data]

fail_msg:   db 'Long Mode not supported. Halting!', 0
fail_msg_len:   equ $ - fail_msg

; Define a Long Mode Global Descriptor Table
GDT64:
    .Null: equ $ - GDT64
    dw 0x0000, 0x0000
    db 0x00, 0x00, 0x00, 0x00

    .Code: equ $ - GDT64
    dw 0x0000, 0x0000
    db 0x00, 0x9a, 0x2f, 0x00

    .Data: equ $ - GDT64
    dw 0x0000, 0x0000
    db 0x00, 0x92, 0x0f, 0x00

    .Pointer:
    dw $ - GDT64 - 1
    dq GDT64


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Text Section
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
[section .text]

kernel_boot:
    cli
    ;;;;;;;;;;;;;; Save Multiboot Information ;;;;;;;;;;;;;;
    push ebx    ; Address of Multiboot information
    push eax    ; Multiboot bootloader magic number

    ;;;;;;;;;;;;;;; Check Long Mode Support ;;;;;;;;;;;;;;;
    mov eax, 0x80000001
    cpuid
    test edx, (1 << 29)
    jz .NoLongMode

    ;;;;;;;;;;;;;;;;;;;;; Clear Screen ;;;;;;;;;;;;;;;;;;;;;
    mov edi, 0x000b8000
    mov eax, 0x0f000f00
    mov ecx, 1000
    rep stosd

    ;;;;;;;;;;;;;;;;;;; Setup Long Mode ;;;;;;;;;;;;;;;;;;;
    ; Disable Paging
    mov eax, cr0
    and eax, 0x7fffffff
    mov cr0, eax

    ; Setup Paging
    mov edi, 0x00011000                 ; PML4T base address
    mov cr3, edi
    xor eax, eax
    mov ecx, 4096
    rep stosd
    mov edi, cr3                        ; Points at 0x00011000

    ; Form the Hierarchy
    mov DWORD [edi], 0x00012003         ; PML4T[0] -> PDPT
    add edi, 0x00001000                 ; Now points to 0x00012000 (PDPT)
    mov DWORD [edi], 0x00013003         ; PDPT[0] -> PDT
    add edi, 0x00001000                 ; Now points to 0x00013000 (PDT)
    mov DWORD [edi+0x00], 0x00014003    ; PDT[0] -> PT 1
    mov DWORD [edi+0x08], 0x00015003    ; PDT[1] -> PT 2
    mov DWORD [edi+0x10], 0x00016003    ; PDT[2] -> PT 3
    mov DWORD [edi+0x18], 0x00017003    ; PDT[3] -> PT 4
    mov DWORD [edi+0x20], 0x00018003    ; PDT[4] -> PT 5
    mov DWORD [edi+0x28], 0x00019003    ; PDT[5] -> PT 6
    mov DWORD [edi+0x30], 0x0001a003    ; PDT[6] -> PT 7
    mov DWORD [edi+0x38], 0x0001b003    ; PDT[7] -> PT 8
    mov DWORD [edi+0x40], 0x0001c003    ; PDT[8] -> PT 9
    mov DWORD [edi+0x48], 0x0001d003    ; PDT[9] -> PT 10
    add edi, 0x00001000                 ; Now points to 0x00014000 (PT 1)

    ; Identity Mapping
    mov ebx, 0x00000003     ; First physical page frame base address
    mov ecx, 5120           ; Map first 20 Megabytes

    .setEntry:
    mov DWORD [edi], ebx    ; Map the frame into the entry
    add ebx, 0x00001000     ; Move to the next frame
    add edi, 0x08           ; Move to the next entry
    loop .setEntry

    ; Enable PAE
    mov eax, cr4
    or eax, (1 << 5)    ; CR4.PAE
    mov cr4, eax

    ; Enable Long Mode
    mov ecx, 0xc0000080 ; Specify MSR (EFER)
    rdmsr
    or eax, (1 << 8)    ; EFER.LME
    wrmsr

    ; Enable Paging
    mov eax, cr0
    or eax, (1 << 31)   ; CR0.PG
    mov cr0, eax

    ; Load GDT and far jump
    lgdt [GDT64.Pointer]
    jmp GDT64.Code:kernel_jump  ; Activates Long Mode

    ;;;;;;;;;;;;;;;;;; No Long Mode support ;;;;;;;;;;;;;;;;;;
    .NoLongMode:
    mov eax, 0x000b8000
    mov ecx, fail_msg_len
    mov edx, fail_msg

    .loop1:
    cmp ecx, 0
    je .continue1

    mov bl, BYTE [edx]
    mov BYTE [eax], bl
    inc eax

    mov BYTE [eax], 0x4f
    inc eax

    inc edx
    dec ecx
    jmp .loop1

    .continue1:
    hlt


[bits 64]
kernel_jump:
    ;;;;;;;;;;;;;; Reload Segment Registers ;;;;;;;;;;;;;;
    cli
    mov ax, GDT64.Data
    mov fs, ax
    mov gs, ax
    mov ax, GDT64.Null
    mov ds, ax
    mov es, ax
    mov ss, ax

    ;;;;;;;;;;;;; Extract Multiboot information ;;;;;;;;;;;;;
    pop rax
    mov rbx, rax

    mov rdx, 0x00000000ffffffff
    and rax, rdx    ; Preserve the magic number

    mov rdx, 0xffffffff00000000
    and rbx, rdx    ; Preserve the info address
    shr rbx, 32

    xor rcx, rcx
    xor rdx, rdx

    ;;;;;;;;;;;;;;;;;;;; Begin Kerneling ;;;;;;;;;;;;;;;;;;;;
    mov rsp, stack_top  ; Setup the stack
    mov rbp, stack_top

    mov rdi, rax    ; Pass arguments as per the
    mov rsi, rbx    ; x86-64 calling convention
    call init_kernel    ; Enter the Kernel

    cli
    hlt
