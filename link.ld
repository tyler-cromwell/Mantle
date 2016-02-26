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

OUTPUT_FORMAT (elf64-x86-64)
ENTRY (kernel_boot)

SECTIONS {
    KERNEL_VMA = 0x0000000000000000;    /* 0MB */
    /*KERNEL_VMA = 0x0000000001000000;  /* 16MB */
    KERNEL_LMA = 0x0000000001000000;    /* 16MB */

    . = KERNEL_LMA;

    /* Bootstrap code */
    .boot : {
        BOOTSTRAP = .;
        *boot* (.multiboot) /* Mulitboot Header */
        *boot* (.text)
        *boot* (.data)
        *boot* (.bss)
    }

    . += KERNEL_VMA;

    /* Consolidate all text sections */
    .text : AT(ADDR(.text) - KERNEL_VMA) {
        SECTION_TEXT = .;
        *(EXCLUDE_FILE(*boot) .text)
        *(.rodata*)
        . = ALIGN(0x1000);
    }

    /* Consolidate all data sections */
    .data : AT(ADDR(.data) - KERNEL_VMA) {
        SECTION_DATA = .;
        *(EXCLUDE_FILE(*boot) .data)
        . = ALIGN(0x1000);
    }

    .eh_frame : AT(ADDR(.eh_frame) - KERNEL_VMA) {
        *(.eh_frame)
        . = ALIGN(0x1000);
    }

    /* Consolidate all bss sections */
    .bss : AT(ADDR(.bss) - KERNEL_VMA) {
        SECTION_BSS = .;
        *(EXCLUDE_FILE(*boot) .bss)
        *(COMMON)
        . = ALIGN(0x1000);
    }

    /DISCARD/ : {
       *(.comment)
    }

    KERNEL_END = .;
    KERNEL_SIZE = . - KERNEL_VMA - KERNEL_LMA;
}
