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
   these do not need to link against libc */
#include <stdint.h>

/* Kernel Headers */
#include <drivers/console.h>

/*
 * Obtains the CPU vendor string.
 * Argument:
 *   char *id: Pointer to the location to save the vendor id.
 */
void cpuid_vendor(char *id) {
    uint64_t rax = 0, rbx = 0, rcx = 0, rdx = 0;

    /* Pass 'eax' into register A, then run CPUID */
    __asm__("cpuid": : "a" (rax));

    /* Get the value of registers B, C, and D */
    __asm__ volatile ("": "=b" (rbx));
    __asm__ volatile ("": "=c" (rcx));
    __asm__ volatile ("": "=d" (rdx));
    
    /* Get the vendor id of the CPU */
    for (uint8_t i = 0; i < 4; i++) {
        id[i+0] = rbx >> (i * 8);
        id[i+4] = rdx >> (i * 8);
        id[i+8] = rcx >> (i * 8);
    }
}

/*
 * Obtains the number of processors.
 * Returns:
 *   The number of processors.
 */
uint64_t cpuid_cpus(void) {
    uint64_t rbx = 0;

    /* Pass '0x1' into register A, then run CPUID */
    __asm__ volatile ("cpuid": : "a" (0x1));

    /* Get the value of register B */
    __asm__ volatile ("": "=b" (rbx));

    return (rbx >> 16) & 0xff;
}
