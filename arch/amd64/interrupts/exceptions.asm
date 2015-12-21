;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Copyright (C) 2015 Tyler Cromwell <tyler@csh.rit.edu>
;
; This file is part of Ritchie.
;
; Ritchie is free software: you can redistribute it and/or modify
; it under Version 2 of the terms of the GNU General Public License
; as published by the Free Software Foundation.
;
; Ritchie is distributed in the hope that it will be useful,
; but WITHOUT ANY WARRANTY; without even the implied warranty of
; MERCHANTABILITY of FITNESS FOR A PARTICULAR PURPOSE. See the
; GNU General Public License for more details.
;
; You should have received a copy of the GNU General Public License
; along with Ritchie.
; If not, see <http://www.gnu.org/licenses/old-licenses/gpl-2.0.html>
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

; Global Symbols
global exc00    ; Division By Zero
global exc01    ; Debug
global exc02    ; Non-Maskable Interrupt
global exc03    ; Breakpoint
global exc04    ; Overflow
global exc05    ; Bounds Check
global exc06    ; Invalid Opcode
global exc07    ; Coprocessor Not Available
global exc08    ; Double Fault
global exc10    ; Invalid TSS
global exc11    ; Segment Not Present
global exc12    ; Stack Fault
global exc13    ; General Protection
global exc14    ; Page Fault
global exc16    ; x87 Floating Point
global exc17    ; Alignment Check
global exc18    ; Machine Check
global exc19    ; SIMD Floating Point
global exc30    ; Security

; External Symbols
extern idt_exception_handler


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Text Section
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
[section .text]

exc00:
    cli
    push 0  ; (Dummy) Error Code
    push 0  ; Interrupt Vector
    jmp common_handler


exc01:
    cli
    push 0  ; (Dummy) Error Code
    push 1  ; Interrupt Vector
    jmp common_handler


exc02:
    cli
    push 0  ; (Dummy) Error Code
    push 2  ; Interrupt Vector
    jmp common_handler


exc03:
    cli
    push 0  ; (Dummy) Error Code
    push 3  ; Interrupt Vector
    jmp common_handler


exc04:
    cli
    push 0  ; (Dummy) Error Code
    push 4  ; Interrupt Vector
    jmp common_handler


exc05:
    cli
    push 0  ; (Dummy) Error Code
    push 5  ; Interrupt Vector
    jmp common_handler


exc06:
    cli
    push 0  ; (Dummy) Error Code
    push 6  ; Interrupt Vector
    jmp common_handler


exc07:
    cli
    push 0  ; (Dummy) Error Code
    push 7  ; Interrupt Vector
    jmp common_handler


exc08:
    cli
    ; Error code is already pushed
    push 8  ; Interrupt Vector
    jmp common_handler


exc10:
    cli
    ; Error code is already pushed
    push 10 ; Interrupt Vector
    jmp common_handler


exc11:
    cli
    ; Error code is already pushed
    push 11 ; Interrupt Vector
    jmp common_handler


exc12:
    cli
    ; Error code is already pushed
    push 12 ; Interrupt Vector
    jmp common_handler


exc13:
    cli
    ; Error code is already pushed
    push 13 ; Interrupt Vector
    jmp common_handler


exc14:
    cli
    ; Error code is already pushed
    push 14 ; Interrupt Vector
    jmp common_handler


exc16:
    cli
    push 0  ; (Dummy) Error Code
    push 16 ; Interrupt Vector
    jmp common_handler


exc17:
    cli
    ; Error code is already pushed
    push 17 ; Interrupt Vector
    jmp common_handler


exc18:
    cli
    push 0  ; (Dummy) Error Code
    push 18 ; Interrupt Vector
    jmp common_handler


exc19:
    cli
    push 0  ; (Dummy) Error Code
    push 19 ; Interrupt Vector
    jmp common_handler


exc30:
    cli
    ; Error code is already pushed
    push 30 ; Interrupt Vector
    jmp common_handler


; Common (ASM level) Exception handler.
common_handler:
    ; Save register values
    push rax
    push rbx
    push rcx
    push rdx
    push rbp
    push rsi
    push rdi
    push rsp
    push r8
    push r9
    push r10
    push r11
    push r12
    push r13
    push r14
    push r15

    ; Save data segment
    mov rax, ds
    push rax

    ; Load kernel data segment
    mov ax, 0x10
    mov fs, ax
    mov gs, ax

    ; Pass arguments then call C handler
    mov rdi, [rsp+136]  ; Interrupt Vector
    mov rsi, [rsp+144]  ; Error Code
    call idt_exception_handler

    ; Restore data segment
    pop rax
    mov fs, ax
    mov gs, ax

    ; Restore register values
    pop r15
    pop r14
    pop r13
    pop r12
    pop r11
    pop r10
    pop r9
    pop r8
    pop rsp
    pop rdi
    pop rsi
    pop rbp
    pop rdx
    pop rcx
    pop rbx
    pop rax

    ; Cleanup and return
    add rsp, 16 ; Cleanup Interrupt Vector
                ; and Error Code

    hlt ; Temporary?
    ;sti
    ;iretq
