/* Copyright (C) 2015 Tyler Cromwell <tyler@csh.rit.edu>

   This file is part of Ritchie.

   Ritchie is free software: you can redistribute it and/or modify
   it under Version 2 of the terms of the GNU General Public License
   as published by the Free Software Foundation.

   Ritchie is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY of FITNESS FOR A PARTICULAR PURPOSE. See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with Ritchie.
   If not, see <http://www.gnu.org/licenses/old-licenses/gpl-2.0.html>
*/

#ifndef X86_H
#define X86_H

#include <stdint.h>

struct registers {
    uint32_t gs, fs, es, ds;
    uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;
    uint32_t int_no, err_code;
    uint32_t eip, cs, eflags, useresp, ss;
};

static char *exception_names[] = {
    "Division By Zero",                 /* isr0 */
    "Debug",                            /* isr1 */
//  "Non-maskable Interrupt",           /* isr2 */
//  "Breakpoint",                       /* isr3 */
//  "Overflow",                         /* isr4 */
//  "Bounds Check",                     /* isr5 */
//  "Invalid Opcode",                   /* isr6 */
//  "Coprocessor Not Available",        /* isr7 */
    "Double Fault",                     /* isr8 */
//  "Coprocessor Segment Overrun",      /* isr9 */
//  "Invalid TSS",                      /* isr10 */
//  "Segment Not Present",              /* isr11 */
//  "Stack Fault",                      /* isr12 */
    "General Protection Violation",     /* isr13 */
//  "Page Fault",                       /* isr14 */
};

/* CPUID */
extern void cpuid_vendor(char*);

/* Global Descriptor Table */
extern void gdt_init(void);

/* Interrupt Descriptor Table */
extern void idt_init(void);
extern void idt_install_isrs(void);

#endif
