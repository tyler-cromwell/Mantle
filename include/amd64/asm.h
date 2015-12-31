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

#ifndef AMD64_ASM_H
#define AMD64_ASM_H

/* C Standard Library Headers,
   these don't need to link against libc */
#include <stdint.h>

/* Control Register numbers */
#define CR0 0
#define CR2 2
#define CR3 3
#define CR4 4
#define CR8 8

/* Control Register masks, discards unused bits */
#define CR0_MASK    0x00000e005003f
#define CR3_MASK    0xffffffffff018
#define CR4_MASK    0x00000000507ff
#define CR8_MASK    0x00000003f7fd7

/*
 * Copies the value of a Control Register.
 * Returns:
 *   the value of a Control Register.
 */
static uint64_t rcr(uint8_t n) {
    uint64_t cr = 0;

    switch (n) {
        case 0: asm volatile ("mov %%cr0, %0" : "=r" (cr)); break;
        case 2: asm volatile ("mov %%cr2, %0" : "=r" (cr)); break;
        case 3: asm volatile ("mov %%cr3, %0" : "=r" (cr)); break;
        case 4: asm volatile ("mov %%cr4, %0" : "=r" (cr)); break;
        case 8: asm volatile ("mov %%cr8, %0" : "=r" (cr)); break;
    }

    return cr;
}

/*
 * Writes a value into a Control Register.
 * Arguments:
 *   uint8_t cr: The number of the register to write to.
 *   uint64_t value: The value to write.
 */
static void wcr(uint8_t cr, uint64_t value) {
    uint64_t v = 0;

    switch (cr) {
        case 0:
            v = (value & CR0_MASK);
            asm volatile ("mov %0, %%cr0" :: "r" (v));
            break;
        case 3:
            v = (value & CR3_MASK);
            asm volatile ("mov %0, %%cr3" :: "r" (v));
            break;
        case 4:
            v = (value & CR4_MASK);
            asm volatile ("mov %0, %%cr4" :: "r" (v));
            break;
        case 8:
            v = (value & CR8_MASK);
            asm volatile ("mov %0, %%cr8" :: "r" (v));
            break;
    }
}

/*
 * Read a byte from an I/O port.
 * Arguments:
 *   uint16_t port: The port to read from.
 * Returns:
 *   the byte read from the port.
 */
static inline uint8_t inb(uint16_t port) {
    uint8_t value = 0;
    asm volatile ("inb %1, %0": "=a"(value): "Nd"(port));
    return value;
}

/*
 * Write a byte to an I/O port.
 * Arguments:
 *   uint16_t port: The port to write to.
 *   uint8_t value: The value to write.
 */
static inline void outb(uint16_t port, uint8_t value) {
    asm volatile ("outb %0, %1": : "a"(value), "Nd"(port));
}

#endif
