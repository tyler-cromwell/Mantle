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

/* C Standard Library Headers,
   these don't need link against libc */
#include <stdint.h>

/* Kernel Headers */
#include <drivers/console.h>
#include <kernel/mem.h>
#include <x86/x86.h>
#include "isrs.h"
#include "irqs.h"

/* An IDT interrupt gate */
struct IdtGate {
    uint16_t offset_low;    /* Lower portion of the offset */
    uint16_t selector;      /* Interrupt selector */
    uint8_t zero;           /* Always zero */
    uint8_t type_attr;      /* Type attributes */
    uint16_t offset_high;   /* Upper portion of the offset */
} __attribute__((__packed__));

/* The IDTR register */
struct Idtr {
    uint16_t limit; /* The length of the IDT */
    uint32_t base;  /* IDT base address */
} __attribute__((__packed__));

/* */
struct SelectorError {
    uint16_t ext : 1;
    uint16_t idt : 1;
    uint16_t ti : 1;
    uint16_t index : 13;
    uint16_t reserved;
} __attribute__((__packed__));

/* */
struct PageFaultError {
    uint32_t p : 1;
    uint32_t rw : 1;
    uint32_t us : 1;
    uint32_t rsv : 1;
    uint32_t id : 1;
    uint32_t reserved : 27;
} __attribute__((__packed__));

/* The Interrupt Descriptor Table */
static struct IdtGate idt[256];

/* External - Load the IDT */
extern void idt_load(struct Idtr);

/*
 * Creates a new interrupt gate.
 * Arguments:
 *   uint8_t index: The index into the IDT.
 *   uint32_t; Descriptor base address.
 *   uint16_t: A code segment selector.
 *   uint8_t: Type attributes.
 */
static void idt_set_gate(uint8_t index, uint32_t base, uint16_t selector, uint8_t type_attr) {
    struct IdtGate gate = {
        .offset_low = (base & 0xffff),
        .selector = selector,
        .zero = 0,
        .type_attr = type_attr,
        .offset_high = (base >> 16) & 0xffff
    };

    idt[index] = gate;
}

/*
 * Installs the Interrupt Descriptor Table and
 * Remaps the Programmable Interrupt Controller.
 */
void idt_init(void) {
    /* Setup Pointer */
    struct Idtr idtr = {
        .limit = (sizeof(struct Idtr) * 256) - 1,
        .base = (uint32_t) &idt
    };

    /* Load an empty IDT */
    memset((void*) &idt, 0, sizeof(struct IdtGate) * 256);
    idt_load(idtr);

    console_printf(FG_WHITE, "IDT Initialized\n");
}

/*
 * Installs the system Interrupt Service routines.
 * isr9 - Coprocessor Segment Overrun - RESERVED
 * isr 20-29, 31 - RESERVED
 */
void idt_install_isrs(void) {
    idt_set_gate( 0, (unsigned)  isr0, 0x08, 0x8e); /* Division by Zero (Fault - Precise) */
    idt_set_gate( 1, (unsigned)  isr1, 0x08, 0x8e); /* Debug (Fault/Trap - Precise) */
//  idt_set_gate( 2, (unsigned)  isr2, 0x08, 0x8e); /* Non-maskable Interrupt */
//  idt_set_gate( 3, (unsigned)  isr3, 0x08, 0x8e); /* Breakpoint (Trap - Precise) */
//  idt_set_gate( 4, (unsigned)  isr4, 0x08, 0x8e); /* Overflow (Trap - Precise) */
//  idt_set_gate( 5, (unsigned)  isr5, 0x08, 0x8e); /* Bounds Check (Fault - Precise) */
//  idt_set_gate( 6, (unsigned)  isr6, 0x08, 0x8e); /* Invalid Opcode (Fault - Precise) */
//  idt_set_gate( 7, (unsigned)  isr7, 0x08, 0x8e); /* Device Not Available (Fault - Precise) */
    idt_set_gate( 8, (unsigned)  isr8, 0x08, 0x8e); /* Double Fault (Abort - Imprecise) */
//  idt_set_gate(10, (unsigned) isr10, 0x08, 0x8e); /* Invalid TSS (Fault - Precise) */
//  idt_set_gate(11, (unsigned) isr11, 0x08, 0x8e); /* Segment Not Present (Fault - Precise) */
//  idt_set_gate(12, (unsigned) isr12, 0x08, 0x8e); /* Stack Fault (Fault - Precise) */
    idt_set_gate(13, (unsigned) isr13, 0x08, 0x8e); /* General Protect Violation (Fault - Precise) */
//  idt_set_gate(14, (unsigned) isr14, 0x08, 0x8e); /* Page Fault (Fault - Precise) */
//  idt_set_gate(16, (unsigned) isr16, 0x08, 0x8e); /* x87 Floating Point Exception-Pending (Fault - Imprecise) */
//  idt_set_gate(17, (unsigned) isr17, 0x08, 0x8e); /* Alignment Check (Fault - Precise) */
//  idt_set_gate(18, (unsigned) isr18, 0x08, 0x8e); /* Machine-Check (Abort - Imprecise) */
//  idt_set_gate(19, (unsigned) isr19, 0x08, 0x8e); /* SIMD Floating-Point (Fault - Precise) */
//  idt_set_gate(30, (unsigned) isr19, 0x08, 0x8e); /* Security Exception (- Precise) */
    console_printf(FG_WHITE, "ISRs installed\n");
}

/*
 * Installs the Interrupt Request handlers.
 */
void idt_install_irqs(void) {
    idt_set_gate(32, (unsigned)  irq0, 0x08, 0x8e); /* ??? */
    idt_set_gate(33, (unsigned)  irq1, 0x08, 0x8e); /* ??? */
//  idt_set_gate(35, (unsigned)  irq3, 0x08, 0x8e); /* ??? */
//  idt_set_gate(36, (unsigned)  irq4, 0x08, 0x8e); /* ??? */
//  idt_set_gate(37, (unsigned)  irq5, 0x08, 0x8e); /* ??? */
//  idt_set_gate(38, (unsigned)  irq6, 0x08, 0x8e); /* ??? */
//  idt_set_gate(39, (unsigned)  irq7, 0x08, 0x8e); /* ??? */
//  idt_set_gate(40, (unsigned)  irq8, 0x08, 0x8e); /* ??? */
//  idt_set_gate(41, (unsigned)  irq9, 0x08, 0x8e); /* ??? */
//  idt_set_gate(42, (unsigned) irq10, 0x08, 0x8e); /* ??? */
//  idt_set_gate(43, (unsigned) irq11, 0x08, 0x8e); /* ??? */
//  idt_set_gate(44, (unsigned) irq12, 0x08, 0x8e); /* ??? */
//  idt_set_gate(45, (unsigned) irq13, 0x08, 0x8e); /* ??? */
//  idt_set_gate(46, (unsigned) irq14, 0x08, 0x8e); /* ??? */
    idt_set_gate(47, (unsigned) irq15, 0x08, 0x8e); /* ??? */
    console_printf(FG_WHITE, "IRQs installed\n");
}

void idt_handle_isr(struct registers *regs) {}
void idt_handle_irq(struct registers *regs) {}
