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
    push byte 0     ; Dummy error code
    push byte 0     ; Interrupt vector
    jmp common_handler


; Debug
exc01:
    cli
    push byte 0     ; Dummy error code
    push byte 1     ; Interrupt vector
    jmp common_handler


exc02:
    cli
    push byte 2     ; Interrupt vector
    jmp common_handler


exc03:
    cli
    push byte 3     ; Interrupt vector
    jmp common_handler


exc04:
    cli
    push byte 4     ; Interrupt vector
    jmp common_handler


exc05:
    cli
    push byte 5     ; Interrupt vector
    jmp common_handler


exc06:
    cli
    push byte 6     ; Interrupt vector
    jmp common_handler


exc07:
    cli
    push byte 7     ; Interrupt vector
    jmp common_handler


; Double Fault
exc08:
    cli
    push byte 8     ; Interrupt vector
    jmp common_handler


exc10:
    cli
    push byte 10    ; Interrupt vector
    jmp common_handler


exc11:
    cli
    push byte 11    ; Interrupt vector
    jmp common_handler


exc12:
    cli
    push byte 12    ; Interrupt vector
    jmp common_handler


; General Protection
exc13:
    cli
    push byte 13    ; Interrupt vector
    jmp common_handler


; Page Fault
exc14:
    cli
    push byte 14    ; Interrupt vector
    jmp common_handler


exc16:
    cli
    push byte 16     ; Interrupt vector
    jmp common_handler


exc17:
    cli
    push byte 17    ; Interrupt vector
    jmp common_handler


exc18:
    cli
    push byte 18    ; Interrupt vector
    jmp common_handler


exc19:
    cli
    push byte 19     ; Interrupt vector
    jmp common_handler


exc30:
    cli
    push byte 30     ; Interrupt vector
    jmp common_handler


; Common Interrupt handler stub
common_handler:
    hlt
;    pushad

;    mov ax, ds
;    push rax

;    mov ax, 0x10
;    mov ds, ax
;    mov es, ax
;    mov fs, ax
;    mov gs, ax

    call idt_exception_handler

;    pop rax
;    mov ds, ax
;    mov es, ax
;    mov fs, ax
;    mov gs, ax

;    popad
;    add rsp, 16 ;???
;    sti
    iretq
