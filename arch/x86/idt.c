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
void idt_set_gate(uint8_t index, uint32_t base, uint16_t selector, uint8_t type_attr) {
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
