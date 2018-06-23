;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;  Copyright (C) 2015-Present Tyler Cromwell <tjc6185@gmail.com>
;
;  This file is part of Mantle.
;
;  Mantle is free software: you can redistribute it and/or modify
;  it under Version 2 of the terms of the GNU General Public License
;  as published by the Free Software Foundation.
;
;  Mantle is distributed in the hope that it will be useful,
;  but WITHOUT ANY WARRANTY; without even the implied warranty of
;  MERCHANTABILITY of FITNESS FOR A PARTICULAR PURPOSE. See the
;  GNU General Public License for more details.
;
;  You should have received a copy of the GNU General Public License
;  along with Mantle.
;  If not, see <http://www.gnu.org/licenses/old-licenses/gpl-2.0.html>
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

[bits 32]

; Global Symbols
global kernel_boot

; External Symbols
extern KERNEL_SIZE
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
    align 4
    stack_bottom:
    resb 16384
    stack_top:


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Data Section
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
[section .data]

fail_msg:       db 'Long Mode not supported. Halting!', 0
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

    ;;;;;;;;;;;;; Clear Guaranteed Free Memory ;;;;;;;;;;;;;
    ; 0x00000500 -> 0x00007bff
    mov edi, 0x00000500
    mov eax, 0x00000000
    mov ecx, 7616
    rep stosd

    ; 0x00007e00 -> 0x0000ffff
    mov edi, 0x00007e00
    mov eax, 0x00000000
    mov ecx, 8320
    rep stosd

    ; 0x00011000 -> 0x0005ffff
    mov edi, 0x00011000
    mov eax, 0x00000000
    mov ecx, 80896
    rep stosd

    ; 0x000b8000 -> 0x000b8fa0
    mov edi, 0x000b8000
    mov eax, 0x0f000f00
    mov ecx, 1000
    rep stosd

    ;;;;;;;;;;;;;;;;;;; Setup Long Mode ;;;;;;;;;;;;;;;;;;;
    ; Disable Paging
    mov eax, cr0
    and eax, 0x7fffffff
    mov cr0, eax

    ; Determine number of Pages for the kernel
    mov edx, 0
    mov eax, KERNEL_SIZE
    mov ebx, 4096
    div ebx                 ; EAX = Page amount
                            ; EDX = Remainder
    cmp edx, 0
    je .end1
    inc eax                 ; Account for remainder
    .end1:
    push eax                ; Store page amount

    ; Determine number of Pages Tables for the kernel
    mov edx, 0
    ; EAX already contains page amount
    mov ebx, 512
    div ebx                 ; EAX = Page table amount
                            ; EDX = Remainder
    cmp edx, 0
    je .end2
    inc eax                 ; Account for remainder
    .end2:
    push eax                ; Store page table amount

    ; Setup PML4T and CR3
    mov edi, 0x00011000     ; PML4T base address
    mov cr3, edi
    xor eax, eax
    mov ecx, 4096
    rep stosd
    mov edi, cr3            ; Points at 0x00011000

    ; Form the Hierarchy
    mov DWORD [edi], 0x00012003         ; PML4T[0] -> PDPT
    add edi, 0x00001000                 ; Now points to 0x00012000 (PDPT)
    mov DWORD [edi], 0x00013003         ; PDPT[0] -> PDT
    add edi, 0x00001000                 ; Now points to 0x00013000 (PDT)

    ; Tables for the first 16 Megabytes
    mov DWORD [edi+0x00], 0x00014003    ; PDT[0] -> PT 1
    mov DWORD [edi+0x08], 0x00015003    ; PDT[1] -> PT 2
    mov DWORD [edi+0x10], 0x00016003    ; PDT[2] -> PT 3
    mov DWORD [edi+0x18], 0x00017003    ; PDT[3] -> PT 4
    mov DWORD [edi+0x20], 0x00018003    ; PDT[4] -> PT 5
    mov DWORD [edi+0x28], 0x00019003    ; PDT[5] -> PT 6
    mov DWORD [edi+0x30], 0x0001a003    ; PDT[6] -> PT 7
    mov DWORD [edi+0x38], 0x0001b003    ; PDT[7] -> PT 8

    pop eax                 ; Retrieve page table amount
    push edi
    add edi, 0x40
    mov ecx, eax
    mov ebx, 0x0001c003

    ; Create Page Tables to store kernel Pages
    .createPageTable:
    mov DWORD [edi], ebx
    add ebx, 0x00001000
    add edi, 0x08
    loop .createPageTable

    pop edi
    add edi, 0x00001000     ; Now points to 0x00014000 (PT 1)

    ; Identity Mapping
    pop eax                 ; Retrieve page amount
    mov ecx, 4096
    add ecx, eax            ; Add Page amount for first 16 Megabytes
    push ecx
    mov ebx, 0x00000003     ; First physical page frame base address

    .mapFrame:
    mov DWORD [edi], ebx    ; Map the frame into the entry
    add ebx, 0x00001000     ; Move to the next frame
    add edi, 0x08           ; Move to the next entry
    loop .mapFrame

    ; Enable PAE
    mov eax, cr4
    or eax, (1 << 5)        ; CR4.PAE
    mov cr4, eax

    ; Enable Long Mode
    mov ecx, 0xc0000080     ; Specify MSR (EFER)
    rdmsr
    or eax, (1 << 8)        ; EFER.LME
    wrmsr

    ; Enable Paging
    mov eax, cr0
    or eax, (1 << 31)       ; CR0.PG
    mov cr0, eax

    ; Load GDT and far jump
    pop ecx                 ; Kernel Page amount
    pop eax                 ; Bootloader magic number
    pop ebx                 ; Multiboot address
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
    mov di, GDT64.Data
    mov fs, di
    mov gs, di
    mov di, GDT64.Null
    mov ds, di
    mov es, di
    mov ss, di

    ;;;;;;;;;;;;;;;;;;;; Begin Kerneling ;;;;;;;;;;;;;;;;;;;;
    mov rsp, stack_top  ; Setup the stack
    mov rbp, stack_top

    ; Setup arguments (x86-64 CC)
    mov rdi, rax        ; Bootloader magic number
    mov rsi, rbx        ; Multiboot address
    mov rdx, rcx        ; Page amount

    call init_kernel    ; Enter the Kernel

    cli
    hlt
