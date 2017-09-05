;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;  Copyright (C) 2015 Tyler Cromwell <tjc6185@gmail.com>
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
global irq00    ; i8253 PIT
global irq01    ; PS/2 Keyboard
global irq03    ; COM2
global irq04    ; COM1
global irq05    ; LPT2
global irq06    ; Floppy Disk
global irq07    ; LPT1
global irq08    ; CMOS RTC
global irq09    ; Free for peripherals
global irq10    ; Free for peripherals
global irq11    ; Free for peripherals
global irq12    ; PS/2 Mouse
global irq13    ; FPU / Coprocessor / Inter-processor
global irq14    ; Primary ATA HDD
global irq15    ; Secondary ATA HDD

; External Symbols
extern idt_irq_handler


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Text Section
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
[section .text]

irq00:
    cli
    push 0  ; Dummy Error Code
    push 32 ; Interrupt Vector
    jmp common_handler


irq01:
    cli
    push 0  ; Dummy Error Code
    push 33 ; Interrupt Vector
    jmp common_handler


irq03:
    cli
    push 0  ; Dummy Error Code
    push 35 ; Interrupt Vector
    jmp common_handler


irq04:
    cli
    push 0  ; Dummy Error Code
    push 36 ; Interrupt Vector
    jmp common_handler


irq05:
    cli
    push 0  ; Dummy Error Code
    push 37 ; Interrupt Vector
    jmp common_handler


irq06:
    cli
    push 0  ; Dummy Error Code
    push 38 ; Interrupt Vector
    jmp common_handler


irq07:
    cli
    push 0  ; Dummy Error Code
    push 39 ; Interrupt Vector
    jmp common_handler


irq08:
    cli
    push 0  ; Dummy Error Code
    push 40 ; Interrupt Vector
    jmp common_handler


irq09:
    cli
    push 0  ; Dummy Error Code
    push 41 ; Interrupt Vector
    jmp common_handler


irq10:
    cli
    push 0  ; Dummy Error Code
    push 42 ; Interrupt Vector
    jmp common_handler


irq11:
    cli
    push 0  ; Dummy Error Code
    push 43 ; Interrupt Vector
    jmp common_handler


irq12:
    cli
    push 0  ; Dummy Error Code
    push 44 ; Interrupt Vector
    jmp common_handler


irq13:
    cli
    push 0  ; Dummy Error Code
    push 45 ; Interrupt Vector
    jmp common_handler


irq14:
    cli
    push 0  ; Dummy Error Code
    push 46 ; Interrupt Vector
    jmp common_handler


irq15:
    cli
    push 0  ; Dummy Error Code
    push 47 ; Interrupt Vector
    jmp common_handler


; Common (ASM level) IRQ handler.
common_handler:
    ; Save register values
    push rax
    push rbx
    push rcx
    push rdx
    push rsp
    push rbp
    push rsi
    push rdi
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
    mov rdi, rsp
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
    pop rdi
    pop rsi
    pop rbp
    pop rsp
    pop rdx
    pop rcx
    pop rbx
    pop rax

    ; Cleanup and return
    add rsp, 16 ; Cleanup Interrupt Vector
                ; and Error Code

    sti
    iretq
