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
   these don't need to link against libc */
#include <stdint.h>

/* Kernel Headers */
#include <amd64/amd64.h>
#include <amd64/i8253.h>
#include <amd64/i8259.h>
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

/* External functions */
void disable_apic(void);        /* Defined in "amd64.asm" */
void idt_load(struct Idtr*);    /* Defined in "amd64.asm" */
void keyboard_handler(void);    /* Defined in "keyboard.c" */

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
        .offset_low = (base >> 0) & 0xffff,
        .selector = selector,
        .zero1 = 0,
        .type_attr = type_attr,
        .offset_middle = (base >> 16) & 0xffff,
        .offset_high = (base >> 32) & 0xffffffff,
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
    idt_set_gate( 0, (uint64_t) exc00, 0x08, 0x8e); /* Division by Zero (Fault - Precise) */
    idt_set_gate( 1, (uint64_t) exc01, 0x08, 0x8e); /* Debug (Fault/Trap - Precise) */
    idt_set_gate( 2, (uint64_t) exc02, 0x08, 0x8e); /* Non-maskable Interrupt */
    idt_set_gate( 3, (uint64_t) exc03, 0x08, 0x8e); /* Breakpoint (Trap - Precise) */
    idt_set_gate( 4, (uint64_t) exc04, 0x08, 0x8e); /* Overflow (Trap - Precise) */
    idt_set_gate( 5, (uint64_t) exc05, 0x08, 0x8e); /* Bounds Check (Fault - Precise) */
    idt_set_gate( 6, (uint64_t) exc06, 0x08, 0x8e); /* Invalid Opcode (Fault - Precise) */
    idt_set_gate( 7, (uint64_t) exc07, 0x08, 0x8e); /* Device Not Available (Fault - Precise) */
    idt_set_gate( 8, (uint64_t) exc08, 0x08, 0x8e); /* Double Fault (Abort - Imprecise) */
/*  idt_set_gate( 9, (uint64_t) exc09, 0x08, 0x8e); */
    idt_set_gate(10, (uint64_t) exc10, 0x08, 0x8e); /* Invalid TSS (Fault - Precise) */
    idt_set_gate(11, (uint64_t) exc11, 0x08, 0x8e); /* Segment Not Present (Fault - Precise) */
    idt_set_gate(12, (uint64_t) exc12, 0x08, 0x8e); /* Stack Fault (Fault - Precise) */
    idt_set_gate(13, (uint64_t) exc13, 0x08, 0x8e); /* General Protect (Fault - Precise) */
    idt_set_gate(14, (uint64_t) exc14, 0x08, 0x8e); /* Page Fault (Fault - Precise) */
/*  idt_set_gate(15, (uint64_t) exc15, 0x08, 0x8e); */
    idt_set_gate(16, (uint64_t) exc16, 0x08, 0x8e); /* x87 Floating Point (Fault - Imprecise) */
    idt_set_gate(17, (uint64_t) exc17, 0x08, 0x8e); /* Alignment Check (Fault - Precise) */
    idt_set_gate(18, (uint64_t) exc18, 0x08, 0x8e); /* Machine Check (Abort - Imprecise) */
    idt_set_gate(19, (uint64_t) exc19, 0x08, 0x8e); /* SIMD Floating-Point (Fault - Precise) */
/*  idt_set_gate(20, (uint64_t) exc20, 0x08, 0x8e); */
/*  idt_set_gate(21, (uint64_t) exc21, 0x08, 0x8e); */
/*  idt_set_gate(22, (uint64_t) exc22, 0x08, 0x8e); */
/*  idt_set_gate(23, (uint64_t) exc23, 0x08, 0x8e); */
/*  idt_set_gate(24, (uint64_t) exc24, 0x08, 0x8e); */
/*  idt_set_gate(25, (uint64_t) exc25, 0x08, 0x8e); */
/*  idt_set_gate(26, (uint64_t) exc26, 0x08, 0x8e); */
/*  idt_set_gate(27, (uint64_t) exc27, 0x08, 0x8e); */
/*  idt_set_gate(28, (uint64_t) exc28, 0x08, 0x8e); */
/*  idt_set_gate(29, (uint64_t) exc29, 0x08, 0x8e); */
    idt_set_gate(30, (uint64_t) exc30, 0x08, 0x8e); /* Security (- Precise) */
/*  idt_set_gate(31, (uint64_t) exc31, 0x08, 0x8e); */
    console_printf(FG_WHITE, "Exception handlers installed\n");
}

/*
 * Installs the Interrupt Request handlers.
 */
static void idt_install_irq_handlers(void) {
    idt_set_gate(32, (uint64_t) irq00, 0x08, 0x8e); /* i8253 timer */
    idt_set_gate(33, (uint64_t) irq01, 0x08, 0x8e); /* Keyboard */
/*  idt_set_gate(34, (uint64_t) irq02, 0x08, 0x8e);    Slave PIC */
    idt_set_gate(35, (uint64_t) irq03, 0x08, 0x8e); /* ??? */
    idt_set_gate(36, (uint64_t) irq04, 0x08, 0x8e); /* ??? */
    idt_set_gate(37, (uint64_t) irq05, 0x08, 0x8e); /* ??? */
    idt_set_gate(38, (uint64_t) irq06, 0x08, 0x8e); /* ??? */
    idt_set_gate(39, (uint64_t) irq07, 0x08, 0x8e); /* ??? */
    idt_set_gate(40, (uint64_t) irq08, 0x08, 0x8e); /* ??? */
    idt_set_gate(41, (uint64_t) irq09, 0x08, 0x8e); /* ??? */
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
void idt_configure(void) {
    memset((void*) idt, 0, sizeof(struct IdtGate) * 256);

    /* Initialize controller (using i8259) */
    disable_apic();
    i8259_init();
    i8259_unmask();

    /* Initialize the i8253 timer */
    i8253_init(100);

    /* Setup Pointer */
    struct Idtr idtr = {
        .limit = (sizeof(struct IdtGate) * 256) - 1,
        .base = (uint64_t) idt
    };

    /* Install handlers */
    idt_install_exception_handlers();
    idt_install_irq_handlers();    
    idt_load(&idtr);

    console_printf(FG_WHITE, "IDT setup, interrupts enabled\n\n");
}

void idt_exception_handler(uint64_t vector, uint64_t error) {
    console_printf(FG_BROWN_L, "%s!\n", interrupts[vector]);

    /* If Page Fault */
    if (vector == 14) {
        struct PageFaultError pfe = {0};
        memcpy(&pfe, &error, sizeof(uint16_t));
    }
    /* If Error Code */
    else if (error > 0) {
        struct SelectorError se = {0};
        memcpy(&se, &error, sizeof(uint32_t));
    }
}

void idt_irq_handler(uint64_t vector) {
    /* Send reset signal */
    if (vector >= 40) {
        outb(I8259_SLAVE_CMD, 0x20);
    }
    outb(I8259_MASTER_CMD, 0x20);

    /* Determine specific handler */
    switch (vector) {
        case 32: break;
        case 33: keyboard_handler(); break;
    }
}
