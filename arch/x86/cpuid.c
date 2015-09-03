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

/* C Standard Library Header,
   these do not need to link against libc */
#include <stdint.h>

/*
 * Obtains the CPU vendor string.
 * Argument:
 *   char* id: Pointer to the location to save the vendor id.
 */
void cpuid_vendor(char *id) {
    uint32_t eax = 0, ebx = 0, ecx = 0, edx = 0;

    /* Pass 'eax' into register A, then run CPUID */
    __asm__("cpuid": : "a" (eax));

    /* Get the value of registers B, C, and D */
    __asm__("": "=b" (ebx));
    __asm__("": "=c" (ecx));
    __asm__("": "=d" (edx));
    
    /* Get the vendor id of the CPU */
    for (uint8_t i = 0; i < 4; i++) {
        id[i+0] = ebx >> (i * 8);
        id[i+4] = edx >> (i * 8);
        id[i+8] = ecx >> (i * 8);
    }
}

/*
 * Obtains the number of processor cores.
 * Returns:
 *   The number of cores.
 */
uint32_t cpuid_cores(void) {
    uint32_t ebx = 0;

    /* Pass '0x1' into register A, then run CPUID */
    __asm__ volatile ("cpuid": : "a" (0x1));

    /* Get the value of register B */
    __asm__ volatile ("": "=b" (ebx));

    return (ebx >> 16) & 0xff;
}
