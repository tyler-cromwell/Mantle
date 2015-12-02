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
global irq0
global irq1
global irq3
global irq4
global irq5
global irq6
global irq7
global irq8
global irq9
global irq10
global irq11
global irq12
global irq13
global irq14
global irq15

; External Symbols
extern idt_irq_handler

[section .text]

;
irq0:
    cli
    push byte 0
    push byte 32
    jmp common_handler


; Keyboard?
irq1:
    cli
    push byte 1
    push byte 33
    jmp common_handler


;
irq3:
    cli
    push byte 0
    push byte 35
    jmp common_handler


;
irq4:
    cli
    push byte 0
    push byte 36
    jmp common_handler


;
irq5:
    cli
    push byte 0
    push byte 37
    jmp common_handler


;
irq6:
    cli
    push byte 0
    push byte 38
    jmp common_handler


;
irq7:
    cli
    push byte 0
    push byte 39
    jmp common_handler


;
irq8:
    cli
    push byte 0
    push byte 40
    jmp common_handler


;
irq9:
    cli
    push byte 0
    push byte 41
    jmp common_handler


;
irq10:
    cli
    push byte 0
    push byte 42
    jmp common_handler


;
irq11:
    cli
    push byte 0
    push byte 43
    jmp common_handler


;
irq12:
    cli
    push byte 0
    push byte 44
    jmp common_handler


;
irq13:
    cli
    push byte 0
    push byte 45
    jmp common_handler


;
irq14:
    cli
    push byte 0
    push byte 46
    jmp common_handler


;
irq15:
    cli
    push byte 0
    push byte 47
    jmp common_handler


;
common_handler:
    hlt
    call idt_irq_handler
;    pusha
;    push ds
;    push es
;    push fs
;    push gs
;    mov ax, 0x10
;    mov ds, ax
;    mov es, ax
;    mov fs, ax
;    mov gs, ax
;    mov eax, esp
;    push eax
;    mov eax, idt_irq_handler
;    call eax
;    pop eax
;    pop gs
;    pop fs
;    pop es
;    pop ds
;    popa
;    add esp, 8
;    iret
