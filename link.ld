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

/* Referenced from osdev.org */
OUTPUT_FORMAT(elf32-i386)
ENTRY(start)
SECTIONS {
    . = 0x01000000; /* The address to load the Kernel (16MB in) */

    .text BLOCK(4K) : ALIGN(4K) { *(.text) }
    .data BLOCK(4K) : ALIGN(4K) { *(.data) }

    .bss BLOCK(4K) : ALIGN(4K) {
        *(COMMON)
        *(.bss)
    }

    kernel_size = . - 0x01000000;
}
