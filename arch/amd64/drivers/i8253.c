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
   these don't need to link against libc a*/
#include <stdint.h>

/* Kernel Headers */
#include <amd64/amd64.h>

/*
 * Initializes the Intel 8253
 * Programmable Interval Timer.
 */
void i8253_init(uint8_t frequency) {
    uint32_t d = 1193180 / frequency;
    uint8_t l = d & 0xff;
    uint8_t h = (d >> 8) & 0xff;

    outb(0x42, 0x36);
    outb(0x40, l);
    outb(0x40, h);

    return;
}
