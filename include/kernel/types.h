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

#ifndef KERNEL_TYPES_H
#define KERNEL_TYPES_H

// C Standard Library header(s)
#include <stdint.h>

// Unsigned primitives
typedef unsigned char uchar_t;
typedef unsigned short ushort_t;
typedef unsigned int uint_t;
typedef unsigned long ulong_t;

// Architecture word sizes
typedef uint8_t byte_t;
typedef uint16_t word_t;
typedef uint32_t dword_t;
typedef uint64_t qword_t;

#endif
