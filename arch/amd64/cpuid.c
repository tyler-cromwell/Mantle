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
    uint32_t register eax asm("eax") = 0, ebx asm("ebx") = 0;
    uint32_t register ecx asm("ecx") = 0, edx asm("edx") = 0;

    asm volatile (
        "cpuid"
        : "=b" (ebx), "=c" (ecx), "=d" (edx)
        : "a" (eax)
    );

    /* Get the vendor_id of the CPU */
    for (uint8_t i = 0; i < 4; i++) {
        id[i+0] = ebx >> (i * 8);
        id[i+4] = edx >> (i * 8);
        id[i+8] = ecx >> (i * 8);
    }
}

/*
 * Obtains the number of processors.
 * Returns:
 *   The number of processors.
 */
uint32_t cpuid_cpus(void) {
    uint32_t register eax asm("eax") = 1, ebx asm("ebx") = 0;

    asm volatile (
        "cpuid"
        : "=b" (ebx)
        : "a" (eax)
        : "ecx", "edx"
    );

    return (ebx >> 16) & 0xff;
}
