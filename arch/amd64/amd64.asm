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
global disable_apic
global idt_load

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Text Section
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
[section .text]

; Disables the APIC
disable_apic:
    mov ecx, 0x0000001b
    rdmsr
    and eax, (0 << 11)
    wrmsr
    ret

; Install the IDT
idt_load:
    push rbp
    mov rbp, rsp
    mov rdx, [rbp+8]
    lidt [rdx]
    pop rbp
    sti
    ret
