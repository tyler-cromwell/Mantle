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

/*
 * Read a byte from an I/O port.
 * Arguments:
 *   uint16_t port: The port to read from.
 * Returns:
 *   the byte read from the port.
 */
static inline uint8_t inb(uint16_t port) {
    uint8_t value;
    __asm__ volatile ("inb %1, %0": "=a"(value): "Nd"(port));
    return value;
}

/*
 * Write a byte to an I/O port.
 * Arguments:
 *   uint16_t port: The port to write to.
 *   uint8_t value: The value to write.
 */
static inline void outb(uint16_t port, uint8_t value) {
    __asm__ volatile ("outb %0, %1": : "a"(value), "Nd"(port));
}

#endif
