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

#ifndef AMD64_CPUID_H
#define AMD64_CPUID_H

/* Kernel header(s) */
#include <kernel/types.h>

/* Macro constants */
#define VENDOR_AMD      "AuthenticAMD"
#define VENDOR_INTEL    "GenuineIntel"

/* Function prototypes */
void cpuid_vendor(char *id);
dword_t cpuid_cpus(void);

#endif
