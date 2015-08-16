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

#ifndef X86_INTERRUPTS_H
#define X86_INTERRUPTS_H

/* Name of each internal exception */
static char *exceptions[] = {
    "Division By Zero",                         /*  exc0 */
    "Debug",                                    /*  exc1 */
    "Non-maskable Interrupt",                   /*  exc2 */
    "Breakpoint",                               /*  exc3 */
    "Overflow",                                 /*  exc4 */
    "Bounds Check",                             /*  exc5 */
    "Invalid Opcode",                           /*  exc6 */
    "Coprocessor Not Available",                /*  exc7 */
    "Double Fault",                             /*  exc8 */
    "Reserved",                                 /*  exc9 */
    "Invalid TSS",                              /* exc10 */
    "Segment Not Present",                      /* exc11 */
    "Stack Fault",                              /* exc12 */
    "General Protection Violation",             /* exc13 */
    "Page Fault",                               /* exc14 */
    "Reserved",                                 /* exc15 */
    "x87 Floating Point Exception-Pending",     /* exc16 */
    "Alignment Check",                          /* exc17 */
    "Machine Check",                            /* exc18 */
    "SIMD Floating-Point",                      /* exc19 */
    "Reserved",                                 /* exc20 */
    "Reserved",                                 /* exc21 */
    "Reserved",                                 /* exc22 */
    "Reserved",                                 /* exc23 */
    "Reserved",                                 /* exc24 */
    "Reserved",                                 /* exc25 */
    "Reserved",                                 /* exc26 */
    "Reserved",                                 /* exc27 */
    "Reserved",                                 /* exc28 */
    "Reserved",                                 /* exc29 */
    "Security Exception",                       /* exc30 */
    "Reserved",                                 /* exc31 */
};

/* Exceptions */
extern void exc0(void);
extern void exc1(void);
//extern void exc2(void);
//extern void exc3(void);
//extern void exc4(void);
//extern void exc5(void);
//extern void exc6(void);
//extern void exc7(void);
extern void exc8(void);
//extern void exc10(void);
//extern void exc11(void);
//extern void exc12(void);
extern void exc13(void);
//extern void exc14(void);
//extern void exc16(void);
//extern void exc17(void);
//extern void exc18(void);
//extern void exc19(void);
//extern void exc30(void);

/* Hardware Interrupts */
extern void irq0(void);
extern void irq1(void);
//extern void irq3(void);
//extern void irq4(void);
//extern void irq5(void);
//extern void irq6(void);
//extern void irq7(void);
//extern void irq8(void);
//extern void irq9(void);
//extern void irq10(void);
//extern void irq11(void);
//extern void irq12(void);
//extern void irq13(void);
//extern void irq14(void);
extern void irq15(void);

#endif
