/**********************************************************************
  Copyright (C) 2015 Tyler Cromwell <tjc6185@gmail.com>

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

// C Standard Library header(s)
#include <stdint.h>

// Kernel header(s)
#include <amd64/asm.h>
#include <kernel/types.h>


void i8253_init(byte_t frequency) {
    dword_t d = 1193180 / frequency;
    byte_t l = d & 0xff;
    byte_t h = (d >> 8) & 0xff;

    outb(0x42, 0x36);
    outb(0x40, l);
    outb(0x40, h);

    return;
}
