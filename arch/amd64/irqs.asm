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

; Global Symbols
global irq00    ; i8253 timer
global irq01    ; Keyboard
;      irq02    ; Slave PIC
global irq03
global irq04
global irq05
global irq06
global irq07
global irq08
global irq09
global irq10
global irq11
global irq12
global irq13
global irq14
global irq15
;      irq16
;      irq17
;      irq18
;      irq19
;      irq20
;      irq21
;      irq22
;      irq23
;      irq24
;      irq25
;      irq26
;      irq27
;      irq28
;      irq29
;      irq30
;      irq31

; External Symbols
extern idt_irq_handler


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Text Section
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
[section .text]

irq00:
    cli
    push 0  ; (Dummy) Error Code
    push 32 ; Interrupt Vector
    jmp common_handler


irq01:
    cli
    push 0  ; (Dummy) Error Code
    push 33 ; Interrupt Vector
    jmp common_handler


irq03:
    cli
    push 0  ; (Dummy) Error Code
    push 35 ; Interrupt Vector
    jmp common_handler


irq04:
    cli
    push 0  ; (Dummy) Error Code
    push 36 ; Interrupt Vector
    jmp common_handler


irq05:
    cli
    push 0  ; (Dummy) Error Code
    push 37 ; Interrupt Vector
    jmp common_handler


irq06:
    cli
    push 0  ; (Dummy) Error Code
    push 38 ; Interrupt Vector
    jmp common_handler


irq07:
    cli
    push 0  ; (Dummy) Error Code
    push 39 ; Interrupt Vector
    jmp common_handler


irq08:
    cli
    push 0  ; (Dummy) Error Code
    push 40 ; Interrupt Vector
    jmp common_handler


irq09:
    cli
    push 0  ; (Dummy) Error Code
    push 41 ; Interrupt Vector
    jmp common_handler


irq10:
    cli
    push 0  ; (Dummy) Error Code
    push 42 ; Interrupt Vector
    jmp common_handler


irq11:
    cli
    push 0  ; (Dummy) Error Code
    push 43 ; Interrupt Vector
    jmp common_handler


irq12:
    cli
    push 0  ; (Dummy) Error Code
    push 44 ; Interrupt Vector
    jmp common_handler


irq13:
    cli
    push 0  ; (Dummy) Error Code
    push 45 ; Interrupt Vector
    jmp common_handler


irq14:
    cli
    push 0  ; (Dummy) Error Code
    push 46 ; Interrupt Vector
    jmp common_handler


irq15:
    cli
    push 0  ; (Dummy) Error Code
    push 47 ; Interrupt Vector
    jmp common_handler


; Common Interrupt handler stub
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
    call idt_irq_handler

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
    sti
    iretq
