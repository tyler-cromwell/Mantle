/* Copyright (C) 2015 Tyler Cromwell <tyler@csh.rit.edu>

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
*/

#ifndef KERNEL_STRING_H
#define KERNEL_STRING_H

#include <stddef.h>
#include <stdint.h>

extern char* itoa(uint32_t, uint32_t);
extern int32_t strncmp(const char*, const char*, size_t);
extern char* strncpy(char*, const char*, size_t);
extern size_t strlen(const char*);
extern size_t strnlen(const char*, size_t);

#endif
