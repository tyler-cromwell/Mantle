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
extern void exc00(void);
extern void exc01(void);
extern void exc02(void);
extern void exc03(void);
extern void exc04(void);
extern void exc05(void);
extern void exc06(void);
extern void exc07(void);
extern void exc08(void);
/*          exc09     */
extern void exc10(void);
extern void exc11(void);
extern void exc12(void);
extern void exc13(void);
extern void exc14(void);
/*          exc15     */
extern void exc16(void);
extern void exc17(void);
extern void exc18(void);
extern void exc19(void);
/*          exc20     */
/*          exc21     */
/*          exc22     */
/*          exc23     */
/*          exc24     */
/*          exc25     */
/*          exc26     */
/*          exc27     */
/*          exc28     */
/*          exc29     */
extern void exc30(void);
/*          exc31     */


/* Hardware Interrupts */
extern void irq00(void);
extern void irq01(void);
/*          irq02     */
extern void irq03(void);
extern void irq04(void);
extern void irq05(void);
extern void irq06(void);
extern void irq07(void);
extern void irq08(void);
extern void irq09(void);
extern void irq10(void);
extern void irq11(void);
extern void irq12(void);
extern void irq13(void);
extern void irq14(void);
extern void irq15(void);
/*          irq16     */
/*          irq17     */
/*          irq18     */
/*          irq19     */
/*          irq20     */
/*          irq21     */
/*          irq22     */
/*          irq23     */
/*          irq24     */
/*          irq25     */
/*          irq26     */
/*          irq27     */
/*          irq28     */
/*          irq29     */
/*          irq30     */
/*          irq31     */

#endif
