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
#include "isr.h"

/* An IDT interrupt gate */
struct IdtGate {
    uint16_t offset_low;    /* Lower portion of the offset */
    uint16_t selector;      /* Interrupt selector */
    uint8_t zero;           /* Always zero */
    uint8_t type_attr;      /* Type attributes */
    uint16_t offset_high;   /* Upper portion of the offset */
} __attribute__((__packed__));

/* IDTR register */
struct Idtr {
    uint16_t limit; /* The length of the IDT */
    uint32_t base;  /* IDT base address */
} __attribute__((__packed__));

/* The IDT and a pointer to it */
static struct IdtGate idt[256];
static struct Idtr idtr;

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
 * Installs the Interrupt Descriptor Table.
 */
void idt_init(void) {
    /* Setup Pointer */
    idtr.limit = (sizeof(struct Idtr) * 256) - 1;
    idtr.base = (uint32_t) &idt;

    /* Load an empty IDT */
    memset((void*) &idt, 0, sizeof(struct IdtGate) * 256);
    idt_load(idtr);

    console_printf(FG_WHITE, "IDT Initialized\n");
}

/*
 * Installs the system Interrupt Service Rountines.
 */
void idt_install_isrs(void) {
    idt_set_gate( 0, (unsigned)  isr0, 0x08, 0x8E); /* Divide by Zero */
    idt_set_gate( 1, (unsigned)  isr1, 0x08, 0x8E); /* Debug */
//  idt_set_gate( 2, (unsigned)  isr2, 0x08, 0x8E); /* Non-maskable Interrupt */
//  idt_set_gate( 3, (unsigned)  isr3, 0x08, 0x8E); /* Breakpoint */
//  idt_set_gate( 4, (unsigned)  isr4, 0x08, 0x8E); /* Overflow */
//  idt_set_gate( 5, (unsigned)  isr5, 0x08, 0x8E); /* Bounds Check */
//  idt_set_gate( 6, (unsigned)  isr6, 0x08, 0x8E); /* Invalid Opcode */
//  idt_set_gate( 7, (unsigned)  isr7, 0x08, 0x8E); /* Coprocessor Not Available */
    idt_set_gate( 8, (unsigned)  isr8, 0x08, 0x8E); /* Double Fault */
//  idt_set_gate( 9, (unsigned)  isr9, 0x08, 0x8E); /* Coprocessor Segment Overrun */
//  idt_set_gate(10, (unsigned) isr10, 0x08, 0x8E); /* Invalid TSS */
//  idt_set_gate(11, (unsigned) isr11, 0x08, 0x8E); /* Segment Not Present */
//  idt_set_gate(12, (unsigned) isr12, 0x08, 0x8E); /* Stack Fault */
    idt_set_gate(13, (unsigned) isr13, 0x08, 0x8E); /* General Protect Violation */
//  idt_set_gate(14, (unsigned) isr14, 0x08, 0x8E); /* Page Fault */
//  idt_set_gate(16, (unsigned) isr16, 0x08, 0x8E); /* ??? */
//  idt_set_gate(17, (unsigned) isr17, 0x08, 0x8E); /* ??? */
//  idt_set_gate(18, (unsigned) isr18, 0x08, 0x8E); /* ??? */
//  idt_set_gate(19, (unsigned) isr19, 0x08, 0x8E); /* ??? */
//  idt_set_gate(20, (unsigned) isr20, 0x08, 0x8E); /* If INTEL, virt stuff, else Reserved */
    console_printf(FG_WHITE, "Interrupts Installed\n");
}
