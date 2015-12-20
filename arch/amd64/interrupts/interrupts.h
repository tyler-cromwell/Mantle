/**********************************************************************
  Copyright (C) 2015 Tyler Cromwell <tyler@csh.rit.edu>

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
**********************************************************************/

#ifndef AMD64_INTERRUPTS_H
#define AMD64_INTERRUPTS_H

/* Name of each interrupt */
static char *interrupts[] = {
    "Division By Zero",             /* exc00 */
    "Debug",                        /* exc01 */
    "Non-Maskable Interrupt",       /* exc02 */
    "Breakpoint",                   /* exc03 */
    "Overflow",                     /* exc04 */
    "Bounds Check",                 /* exc05 */
    "Invalid Opcode",               /* exc06 */
    "Coprocessor Not Available",    /* exc07 */
    "Double Fault",                 /* exc08 */
    "Reserved",                     /* exc09 */
    "Invalid TSS",                  /* exc10 */
    "Segment Not Present",          /* exc11 */
    "Stack Fault",                  /* exc12 */
    "General Protection",           /* exc13 */
    "Page Fault",                   /* exc14 */
    "Reserved",                     /* exc15 */
    "x87 Floating Point",           /* exc16 */
    "Alignment Check",              /* exc17 */
    "Machine Check",                /* exc18 */
    "SIMD Floating Point",          /* exc19 */
    "Reserved",                     /* exc20 */
    "Reserved",                     /* exc21 */
    "Reserved",                     /* exc22 */
    "Reserved",                     /* exc23 */
    "Reserved",                     /* exc24 */
    "Reserved",                     /* exc25 */
    "Reserved",                     /* exc26 */
    "Reserved",                     /* exc27 */
    "Reserved",                     /* exc28 */
    "Reserved",                     /* exc29 */
    "Security",                     /* exc30 */
    "Reserved",                     /* exc31 */
};


/* Exceptions */
void exc00(void);
void exc01(void);
void exc02(void);
void exc03(void);
void exc04(void);
void exc05(void);
void exc06(void);
void exc07(void);
void exc08(void);
void exc10(void);
void exc11(void);
void exc12(void);
void exc13(void);
void exc14(void);
void exc16(void);
void exc17(void);
void exc18(void);
void exc19(void);
void exc30(void);


/* Hardware Interrupts */
void irq00(void);
void irq01(void);
void irq03(void);
void irq04(void);
void irq05(void);
void irq06(void);
void irq07(void);
void irq08(void);
void irq09(void);
void irq10(void);
void irq11(void);
void irq12(void);
void irq13(void);
void irq14(void);
void irq15(void);

#endif
