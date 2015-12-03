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

/* C Standard Library Headers,
   these don't need link against libc */
#include <stdint.h>

/* Kernel Headers */
#include <amd64/amd64.h>
#include <drivers/console.h>
#include <kernel/string.h>
#include "interrupts.h"

/* An IDT interrupt gate */
struct IdtGate {
    uint16_t offset_low;    /* Lower portion of the offset */
    uint16_t selector;      /* Interrupt selector */
    uint8_t zero1;          /* Always zero */
    uint8_t type_attr;      /* Type attributes */
    uint16_t offset_middle; /* Middle portion of the offset */
    uint32_t offset_high;   /* Upper portion of the offset */
    uint32_t zero2;         /* Always zero */
} __attribute__((__packed__));

/* The IDTR register */
struct Idtr {
    uint16_t limit; /* The length of the IDT */
    uint64_t base;  /* IDT base address */
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

/* External - Defined in "amd64.asm" */
extern void disable_apic(void);
extern void idt_load(struct Idtr*);

/*
 * Creates a new interrupt gate.
 * Arguments:
 *   uint8_t index: The index into the IDT.
 *   uint32_t; Descriptor base address.
 *   uint16_t: A code segment selector.
 *   uint8_t: Type attributes.
 */
static void idt_set_gate(uint8_t index, uint64_t base, uint16_t selector, uint8_t type_attr) {
    struct IdtGate gate = {
        .offset_low = (base & 0x000000000000ffff),
        .selector = selector,
        .zero1 = 0,
        .type_attr = type_attr,
        .offset_middle = (base & 0x00000000ffff0000),
        .offset_high = (base & 0xffffffff00000000),
        .zero2 = 0
    };

    idt[index] = gate;
}

/*
 * Installs the system exceptions handlers.
 * exc9 - Coprocessor Segment Overrun - RESERVED
 * exc 20-29, 31 - RESERVED
 */
static void idt_install_exception_handlers(void) {
    idt_set_gate( 0, (uint64_t)  exc0, 0x08, 0x8e); /* Division by Zero (Fault - Precise) */
    idt_set_gate( 1, (uint64_t)  exc1, 0x08, 0x8e); /* Debug (Fault/Trap - Precise) */
    idt_set_gate( 2, (uint64_t)  exc2, 0x08, 0x8e); /* Non-maskable Interrupt */
    idt_set_gate( 3, (uint64_t)  exc3, 0x08, 0x8e); /* Breakpoint (Trap - Precise) */
    idt_set_gate( 4, (uint64_t)  exc4, 0x08, 0x8e); /* Overflow (Trap - Precise) */
    idt_set_gate( 5, (uint64_t)  exc5, 0x08, 0x8e); /* Bounds Check (Fault - Precise) */
    idt_set_gate( 6, (uint64_t)  exc6, 0x08, 0x8e); /* Invalid Opcode (Fault - Precise) */
    idt_set_gate( 7, (uint64_t)  exc7, 0x08, 0x8e); /* Device Not Available (Fault - Precise) */
    idt_set_gate( 8, (uint64_t)  exc8, 0x08, 0x8e); /* Double Fault (Abort - Imprecise) */
    idt_set_gate(10, (uint64_t) exc10, 0x08, 0x8e); /* Invalid TSS (Fault - Precise) */
    idt_set_gate(11, (uint64_t) exc11, 0x08, 0x8e); /* Segment Not Present (Fault - Precise) */
    idt_set_gate(12, (uint64_t) exc12, 0x08, 0x8e); /* Stack Fault (Fault - Precise) */
    idt_set_gate(13, (uint64_t) exc13, 0x08, 0x8e); /* General Protect Violation (Fault - Precise) */
    idt_set_gate(14, (uint64_t) exc14, 0x08, 0x8e); /* Page Fault (Fault - Precise) */
    idt_set_gate(16, (uint64_t) exc16, 0x08, 0x8e); /* x87 Floating Point Exception-Pending (Fault - Imprecise) */
    idt_set_gate(17, (uint64_t) exc17, 0x08, 0x8e); /* Alignment Check (Fault - Precise) */
    idt_set_gate(18, (uint64_t) exc18, 0x08, 0x8e); /* Machine Check (Abort - Imprecise) */
    idt_set_gate(19, (uint64_t) exc19, 0x08, 0x8e); /* SIMD Floating-Point (Fault - Precise) */
    idt_set_gate(30, (uint64_t) exc30, 0x08, 0x8e); /* Security Exception (- Precise) */
    console_printf(FG_WHITE, "Exception handlers installed\n");
}

/*
 * Installs the Interrupt Request handlers.
 */
static void idt_install_irq_handlers(void) {
    idt_set_gate(32, (uint64_t)  irq0, 0x08, 0x8e); /* ??? */
    idt_set_gate(33, (uint64_t)  irq1, 0x08, 0x8e); /* ??? */
    idt_set_gate(35, (uint64_t)  irq3, 0x08, 0x8e); /* ??? */
    idt_set_gate(36, (uint64_t)  irq4, 0x08, 0x8e); /* ??? */
    idt_set_gate(37, (uint64_t)  irq5, 0x08, 0x8e); /* ??? */
    idt_set_gate(38, (uint64_t)  irq6, 0x08, 0x8e); /* ??? */
    idt_set_gate(39, (uint64_t)  irq7, 0x08, 0x8e); /* ??? */
    idt_set_gate(40, (uint64_t)  irq8, 0x08, 0x8e); /* ??? */
    idt_set_gate(41, (uint64_t)  irq9, 0x08, 0x8e); /* ??? */
    idt_set_gate(42, (uint64_t) irq10, 0x08, 0x8e); /* ??? */
    idt_set_gate(43, (uint64_t) irq11, 0x08, 0x8e); /* ??? */
    idt_set_gate(44, (uint64_t) irq12, 0x08, 0x8e); /* ??? */
    idt_set_gate(45, (uint64_t) irq13, 0x08, 0x8e); /* ??? */
    idt_set_gate(46, (uint64_t) irq14, 0x08, 0x8e); /* ??? */
    idt_set_gate(47, (uint64_t) irq15, 0x08, 0x8e); /* ??? */
    console_printf(FG_WHITE, "IRQ handlers installed\n");
}

/*
 * Installs the Interrupt Descriptor Table.
 */
void idt_init(void) {
    /* Initialize interrupt controller */
    disable_apic();

    outb(0x20, 0x11);
    outb(0xa0, 0x11);

    outb(0x21, 0x20);
    outb(0xa1, 0x28);

    outb(0x21, 0x00);
    outb(0xa1, 0x00);

    outb(0x21, 0x01);
    outb(0xa1, 0x01);

    outb(0x21, 0xff);
    outb(0xa1, 0xff);

    /* Setup Pointer */
    struct Idtr idtr = {
        .limit = (sizeof(struct IdtGate) * 256) - 1,
        .base = (uint64_t) idt
    };

    /* Install handlers */
    memset((void*) idt, 0, sizeof(struct IdtGate) * 256);

    idt_install_exception_handlers();
    idt_install_irq_handlers();    
    idt_load(&idtr);

    console_printf(FG_WHITE, "IDT Configured\n");
}

void idt_exception_handler(uint32_t *p) {
    p += (9 * 4);
    console_printf(FG_BROWN_L, "Exception %u caught!\n", *p);
    asm volatile ("hlt");
}

void idt_irq_handler(uint32_t *p) {
    p += (9 * 4);
    console_printf(FG_BROWN_L, "Interrupt Request %u caught!\n", *p);
    asm volatile ("hlt");
}
