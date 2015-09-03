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

;
; Important Note:
; This file was heavily referenced from my friends kernel project.
; <https://github.com/WillDignazio/Foundation>
;

bits 32

global exc0
global exc1
global exc8
global exc13

extern idt_handle_exc

exc0:
    cli
    push byte 0
    jmp exc_handle

exc1:
    cli
    push byte 1
    jmp exc_handle

exc8:
    cli
    push byte 8
    jmp exc_handle

exc13:
    cli
    push byte 13
    jmp exc_handle

exc_handle:
    pusha
    push ds
    push es
    push fs
    push gs
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov eax, esp
    push eax
    mov eax, idt_handle_exc
    call eax
    pop eax
    pop gs
    pop fs
    pop es
    pop ds
    popa
    add esp, 8
    iret
