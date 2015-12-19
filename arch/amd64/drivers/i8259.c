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

/* Master PIC */
#define MASTER_CMD  0x20
#define MASTER_DATA 0x21

/* Slave PIC */
#define SLAVE_CMD   0xA0
#define SLAVE_DATA  0xA1

/*
 * Initializes the Intel 8259
 * Programmable Interrupt Controller.
 */
void i8259_init(void) {
    outb(MASTER_CMD, 0x11);
    outb(SLAVE_CMD,  0x11);

    outb(MASTER_DATA, 0x20);
    outb(SLAVE_DATA,  0x28);

    outb(MASTER_DATA, 0x00);
    outb(SLAVE_DATA,  0x00);

    outb(MASTER_DATA, 0x01);
    outb(SLAVE_DATA,  0x01);

    /* Mask all interrupts */
    outb(MASTER_DATA, 0xff);
    outb(SLAVE_DATA,  0xff);
}

/*
 * Masks the given IRQ.
 * Arugment:
 *   uint8_t irq: The IRQ number to mask.
 */
void i8259_set_mask(uint8_t irq) {
    uint16_t port;
    uint8_t value;

    /* Determine PIC */
    if (irq < 8) {
        port = MASTER_DATA;
    } else {
        port = SLAVE_DATA;
        irq -= 8;
    }

    value = inb(port) | (1 << irq);
    outb(port, value);
}

/*
 * Unmasks the given IRQ.
 * Argument:
 *   uint8_t irq: The IRQ number to unmask.
 */
void i8259_clear_mask(uint8_t irq) {
    uint16_t port;
    uint8_t value;

    /* Determine PIC */
    if (irq < 8) {
        port = MASTER_DATA;
    } else {
        port = SLAVE_DATA;
        irq -= 8;
    }

    value = inb(port) & ~(1 << irq);
    outb(port, value);
}

/*
 * Disables all IRQS (masks them).
 */
void i8259_mask(void) {
    outb(MASTER_DATA, 0xff);
    outb(SLAVE_DATA,  0xff);
}

/*
 * Enables all IRQS (unmasks them).
 */
void i8259_unmask(void) {
    outb(MASTER_DATA, 0x00);
    outb(SLAVE_DATA,  0x00);
}
