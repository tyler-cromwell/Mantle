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
global exc00
global exc01
global exc02
global exc03
global exc04
global exc05
global exc06
global exc07
global exc08
;      exc09
global exc10
global exc11
global exc12
global exc13
global exc14
;      exc15
global exc16
global exc17
global exc18
global exc19
;      exc20
;      exc21
;      exc22
;      exc23
;      exc24
;      exc25
;      exc26
;      exc27
;      exc28
;      exc29
global exc30
;      exc31

; External Symbols
extern idt_exception_handler

[section .text]

; Divison By Zero
exc00:
    cli
    push 0  ; (Dummy) Error Code
    push 0  ; Interrupt vector
    jmp common_handler


; Debug
exc01:
    cli
    push 0  ; (Dummy) Error Code
    push 1  ; Interrupt vector
    jmp common_handler


exc02:
    cli
    push 0  ; (Dummy) Error Code
    push 2  ; Interrupt vector
    jmp common_handler


exc03:
    cli
    push 0  ; (Dummy) Error Code
    push 3  ; Interrupt vector
    jmp common_handler


exc04:
    cli
    push 0  ; (Dummy) Error Code
    push 4  ; Interrupt vector
    jmp common_handler


exc05:
    cli
    push 0  ; (Dummy) Error Code
    push 5  ; Interrupt vector
    jmp common_handler


exc06:
    cli
    push 0  ; (Dummy) Error Code
    push 6  ; Interrupt vector
    jmp common_handler


exc07:
    cli
    push 0  ; (Dummy) Error Code
    push 7  ; Interrupt vector
    jmp common_handler


; Double Fault
exc08:
    cli
    ; Error code is already pushed
    push 8  ; Interrupt vector
    jmp common_handler


exc10:
    cli
    ; Error code is already pushed
    push 10 ; Interrupt vector
    jmp common_handler


exc11:
    cli
    ; Error code is already pushed
    push 11 ; Interrupt vector
    jmp common_handler


exc12:
    cli
    ; Error code is already pushed
    push 12 ; Interrupt vector
    jmp common_handler


; General Protection
exc13:
    cli
    ; Error code is already pushed
    push 13 ; Interrupt vector
    jmp common_handler


; Page Fault
exc14:
    cli
    ; Error code is already pushed
    push 14 ; Interrupt vector
    jmp common_handler


exc16:
    cli
    push 0  ; (Dummy) Error Code
    push 16 ; Interrupt vector
    jmp common_handler


exc17:
    cli
    ; Error code is already pushed
    push 17 ; Interrupt vector
    jmp common_handler


exc18:
    cli
    push 0  ; (Dummy) Error Code
    push 18 ; Interrupt vector
    jmp common_handler


exc19:
    cli
    push 0  ; (Dummy) Error Code
    push 19 ; Interrupt vector
    jmp common_handler


exc30:
    cli
    ; Error code is already pushed
    push 30 ; Interrupt vector
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
    mov rdi, [rsp+136]  ; Interrupt vector
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
    sti
    iretq

    ; Evidently.... this returns to the offending instruction
