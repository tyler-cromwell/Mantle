/**********************************************************************
  Copyright (C) 2015-Present Tyler Cromwell <tjc6185@gmail.com>

  This file is part of Mantle.

  Mantle is free software: you can redistribute it and/or modify
  it under Version 2 of the terms of the GNU General Public License
  as published by the Free Software Foundation.

  Mantle is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY of FITNESS FOR A PARTICULAR PURPOSE. See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with Mantle.
  If not, see <http://www.gnu.org/licenses/old-licenses/gpl-2.0.html>
**********************************************************************/

#ifndef AMD64_CPUID_H
#define AMD64_CPUID_H

// Kernel header(s)
#include <kernel/types.h>

// Macro constants
#define VENDOR_AMD      "AuthenticAMD"
#define VENDOR_INTEL    "GenuineIntel"


/*
 * Obtains the CPU vendor string.
 * Argument:
 *   char *id: Location to save the vendor id.
 */
void cpuid_vendor(char *id);


/*
 * Obtains the number of processors.
 */
dword_t cpuid_cpus(void);

#endif
