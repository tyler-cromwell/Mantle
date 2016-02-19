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

/* Kernel header(s) */
#include <amd64/asm.h>
#include <amd64/console.h>
#include <amd64/i8253.h>
#include <amd64/i8259.h>
#include <kernel/types.h>
#include <lib/string.h>

/* Local kernel header(s) */
#include "handlers.h"
#include "idt.h"

/* The Interrupt Descriptor Table */
static struct IdtGate idt[256];

/* External functions */
void keyboard_handler(void);    /* Defined in "keyboard.c" */

/*
 * Disables the APIC.
 */
static void disable_apic(void) {
    dword_t register ecx asm("ecx") = 0x0000001b;
    dword_t msr = 0;

    asm volatile ("rdmsr\n\t" : "=A" (msr));
    msr &= (0 << 11);
    asm volatile ("wrmsr\n\t" : : "A" (msr));
}

/*
 * Installs the given IDT pointer.
 * Argument:
 *   struct Idtr *idtr: A struct representing the IDTR.
 */
static void lidt(struct Idtr *idtr) {
    asm volatile (
        "lidt (%0)\n\t"
        "sti\n\t"
        : : "p" (idtr)
    );
}

/*
 * Creates a new interrupt gate.
 * Arguments:
 *   byte_t index: The index into the IDT.
 *   dword_t; Descriptor base address.
 *   word_t: A code segment selector.
 *   byte_t: Type attributes.
 */
static void idt_set_gate(byte_t index, qword_t base, word_t selector, byte_t type_attr) {
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
 * Installs the system Exception and IRQ handlers.
 * exc9 - Coprocessor Segment Overrun - RESERVED
 * exc 20-29, 31 - RESERVED
 */
static void idt_install_handlers(void) {
    idt_set_gate( 0, (qword_t) exc00, 0x08, 0x8e); /* Division by Zero (Fault - Precise) */
    idt_set_gate( 1, (qword_t) exc01, 0x08, 0x8e); /* Debug (Fault/Trap - Precise) */
    idt_set_gate( 2, (qword_t) exc02, 0x08, 0x8e); /* Non-maskable Interrupt */
    idt_set_gate( 3, (qword_t) exc03, 0x08, 0x8e); /* Breakpoint (Trap - Precise) */
    idt_set_gate( 4, (qword_t) exc04, 0x08, 0x8e); /* Overflow (Trap - Precise) */
    idt_set_gate( 5, (qword_t) exc05, 0x08, 0x8e); /* Bounds Check (Fault - Precise) */
    idt_set_gate( 6, (qword_t) exc06, 0x08, 0x8e); /* Invalid Opcode (Fault - Precise) */
    idt_set_gate( 7, (qword_t) exc07, 0x08, 0x8e); /* Device Not Available (Fault - Precise) */
    idt_set_gate( 8, (qword_t) exc08, 0x08, 0x8e); /* Double Fault (Abort - Imprecise) */
    idt_set_gate(10, (qword_t) exc10, 0x08, 0x8e); /* Invalid TSS (Fault - Precise) */
    idt_set_gate(11, (qword_t) exc11, 0x08, 0x8e); /* Segment Not Present (Fault - Precise) */
    idt_set_gate(12, (qword_t) exc12, 0x08, 0x8e); /* Stack Fault (Fault - Precise) */
    idt_set_gate(13, (qword_t) exc13, 0x08, 0x8e); /* General Protect (Fault - Precise) */
    idt_set_gate(14, (qword_t) exc14, 0x08, 0x8e); /* Page Fault (Fault - Precise) */
    idt_set_gate(16, (qword_t) exc16, 0x08, 0x8e); /* x87 Floating Point (Fault - Imprecise) */
    idt_set_gate(17, (qword_t) exc17, 0x08, 0x8e); /* Alignment Check (Fault - Precise) */
    idt_set_gate(18, (qword_t) exc18, 0x08, 0x8e); /* Machine Check (Abort - Imprecise) */
    idt_set_gate(19, (qword_t) exc19, 0x08, 0x8e); /* SIMD Floating-Point (Fault - Precise) */
    idt_set_gate(30, (qword_t) exc30, 0x08, 0x8e); /* Security (- Precise) */
    console_printf(FG_WHITE, "Exception handlers installed\n");

    idt_set_gate(32, (qword_t) irq00, 0x08, 0x8e); /* i8253 PIT */
    idt_set_gate(33, (qword_t) irq01, 0x08, 0x8e); /* PS/2 Keyboard */
    idt_set_gate(35, (qword_t) irq03, 0x08, 0x8e); /* COM2 */
    idt_set_gate(36, (qword_t) irq04, 0x08, 0x8e); /* COM1 */
    idt_set_gate(37, (qword_t) irq05, 0x08, 0x8e); /* LPT2 */
    idt_set_gate(38, (qword_t) irq06, 0x08, 0x8e); /* Floppy Disk */
    idt_set_gate(39, (qword_t) irq07, 0x08, 0x8e); /* LPT1 */
    idt_set_gate(40, (qword_t) irq08, 0x08, 0x8e); /* CMOS RTC */
    idt_set_gate(41, (qword_t) irq09, 0x08, 0x8e); /* Free for peripherals */
    idt_set_gate(42, (qword_t) irq10, 0x08, 0x8e); /* Free for peripherals */
    idt_set_gate(43, (qword_t) irq11, 0x08, 0x8e); /* Free for peripherals */
    idt_set_gate(44, (qword_t) irq12, 0x08, 0x8e); /* PS/2 Mouse */
    idt_set_gate(45, (qword_t) irq13, 0x08, 0x8e); /* FPU / Coprocessor / Inter-processors */
    idt_set_gate(46, (qword_t) irq14, 0x08, 0x8e); /* Primary ATA HDD */
    idt_set_gate(47, (qword_t) irq15, 0x08, 0x8e); /* Secondary ATA HDD */
    console_printf(FG_WHITE, "IRQ handlers installed\n");
}

/*
 * Dumps the register contents before the Interrupt.
 * Argument:
 *   const struct InterruptStack *const is:
 *     Pointer to the area on the stack
 *     containing the pushed registers.
 */
static void dump_registers(const struct InterruptStack *const is) {
    console_printf(FG_WHITE | BG_RED, "[SS:RSP] %x:%X\n", is->retss, is->retrsp);
    console_printf(FG_WHITE | BG_RED, "rflags=%B\n", is->rflags);
    console_printf(FG_WHITE | BG_RED, "rip=%X\tcs=%X\tds=%X\n", is->rip, is->cs, is->ds);
    console_printf(FG_WHITE | BG_RED, "rax=%X\trbx=%X\trcx=%X\n", is->rax, is->rbx, is->rcx);
    console_printf(FG_WHITE | BG_RED, "rdx=%X\trdi=%X\trsi=%X\n", is->rdx, is->rdi, is->rsi);
    console_printf(FG_WHITE | BG_RED, "rbp=%X\trsp=%X\tr8=%X\n", is->rbp, is->rsp, is->r8);
    console_printf(FG_WHITE | BG_RED, "r9=%X\tr10=%X\tr11=%X\n", is->r9, is->r10, is->r11);
    console_printf(FG_WHITE | BG_RED, "r12=%X\tr13=%X\tr14=%X\n", is->r12, is->r13, is->r14);
    console_printf(FG_WHITE | BG_RED, "r15=%X\n", is->r15);
}

/*
 * Installs the Interrupt Descriptor Table.
 *
 * Only called in "init.c"
 */
void idt_configure(void) {
    memset((void*) idt, 0, sizeof(struct IdtGate) * 256);

    /* Initialize controller (using i8259) */
    disable_apic();
    i8259_init();
    i8259_clear_mask(I8259_IRQ_TIMER);
    i8259_clear_mask(I8259_IRQ_KEYBOARD);

    /* Initialize the i8253 timer */
    i8253_init(100);

    /* Setup Pointer */
    struct Idtr idtr = {
        .limit = (sizeof(struct IdtGate) * 256) - 1,
        .base = (qword_t) idt
    };

    idt_install_handlers();
    lidt(&idtr);
    console_printf(FG_WHITE, "IDT setup, interrupts enabled\n");
}

/*
 * Common (C level) Exception handler.
 *
 * Only called in "exceptions.asm"
 *
 * Argument:
 *   const struct InterruptStack *const is:
 *     Pointer to the area on the stack
 *     containing the pushed registers.
 */
void idt_exception_handler(const struct InterruptStack *const is) {
    console_printf(FG_WHITE | BG_RED, "%s\n", interrupts[is->vector]);

    /* If Page Fault */
    if (is->vector == 14) {
        struct PageFaultError pfe = {0};
        qword_t cr2 = rdcr(CR2);

        memset(&pfe, 0, sizeof(struct PageFaultError));
        memcpy(&pfe, &is->error, sizeof(struct PageFaultError));

        console_printf(FG_WHITE | BG_RED, "PFLA: %x\n", cr2);
        console_printf(FG_WHITE | BG_RED, "P: %u, R/W: %u, U/S: %u, RSV: %u, I/D: %u\n", pfe.p, pfe.rw, pfe.us, pfe.rsv, pfe.id);
    }
    /* If Error Code */
    else if (is->error > 0) {
        struct SelectorError se = {0};

        memset(&se, 0, sizeof(struct SelectorError));
        memcpy(&se, &is->error, sizeof(struct SelectorError));

        console_printf(FG_WHITE | BG_RED, "EXT: %u, IDT: %u, TI: %u, Index: %u\n", se.ext, se.idt, se.ti, se.index);
    }

    dump_registers(is);
    console_printf(FG_WHITE | BG_RED, "System halted");
}

/*
 * Common (C level) IRQ handler.
 *
 * Only called in "irqs.asm"
 *
 * Argument:
 *   const struct InterruptStack *const is:
 *     Pointer to the area on the stack
 *     containing the pushed registers.
 */
void idt_irq_handler(const struct InterruptStack *const is) {
    /* Send reset signal */
    if (is->vector >= 40) {
        outb(I8259_SLAVE_CMD, 0x20);
    }
    outb(I8259_MASTER_CMD, 0x20);

    /* Determine specific handler */
    switch (is->vector) {
        case 32: break;
        case 33: keyboard_handler(); break;
    }
}
