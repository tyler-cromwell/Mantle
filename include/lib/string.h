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

#ifndef LIB_STRING_H
#define LIB_STRING_H

/* C Standard Library header(s) */
#include <stddef.h>

/* Kernel header(s) */
#include <kernel/types.h>


/* Itoa conversion structure */
struct ItoaOptions {
    byte_t pad      : 1;
    byte_t binary   : 1;
    byte_t octal    : 1;
    byte_t hex      : 1;
    byte_t reserved : 4;
} __attribute__((__packed__));


/* Function prototypes */
char* itoa(struct ItoaOptions *opts, long number);
void* memcpy(void *dest, const void *src, size_t n);
void* memset(void *dest, int c, size_t n);
int8_t strlcmp(const char *s1, const char *s2);
int strncmp(const char *s1, const char *s2, size_t n);
char* strncpy(char *dest, const char *src, size_t n);
size_t strlen(const char *string);
size_t strnlen(const char *string, size_t n);
void strlower(char *string);
void strupper(char *string);

#endif
