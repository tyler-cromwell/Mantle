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

#ifndef LIB_STRING_H
#define LIB_STRING_H

// C Standard Library header(s)
#include <stddef.h>

// Kernel header(s)
#include <kernel/types.h>


// Itoa conversion structure
struct ItoaOptions {
    byte_t pad      : 1;
    byte_t binary   : 1;
    byte_t octal    : 1;
    byte_t hex      : 1;
    byte_t reserved : 4;
} __attribute__((__packed__));


/*
 * Creates the string representation of a number.
 * Argument:
 *   struct ItoaOptions *opts:
 *     Pointer to the options that determine
 *     how the number should be converted.
 *   long_t number: The number to convert.
 * Returns:
 *   The string representation of number.
 */
char* itoa(struct ItoaOptions *opts, long number);


/*
 * Copies the contents of one buffer to another.
 * Arguments:
 *   void *dest: The destination buffer.
 *   const void *src: The source buffer.
 *   size_t n: The number of bytes to copy.
 * Returns:
 *   A pointer to the destination buffer.
 */
void* memcpy(void *dest, const void *src, size_t n);


/*
 * Fills the first 'n' bytes of 'dest' with 'c'.
 * Arguments:
 *   void *dest: The region of memory to be over-written.
 *   int c: The value to write with.
 *   size_t n: The number of bytes to write.
 * Returns:
 *   A pointer to the destination buffer.
 */
void* memset(void *dest, int c, size_t n);


/*
 * Compares the contents of 2 strings, taking length into account.
 * Mostly a wrapper for strncmp.
 * Arguments:
 *   const char *s1: The first string.
 *   const char *s2: The second string.
 * Returns:
 *   A status/error code:
 *     1 if equal, 0 if unequal
 *     -1 if s1 is NULL, -2 if s2 is
 */
int8_t strlcmp(const char *s1, const char *s2);


/*
 * Compares the contents of 2 strings. Returns 0 if they are equal.
 * If they are not equal, returns the first character in s1 that is different.
 * Arguments:
 *   const char *s1: The first string.
 *   const char *s2: The second string.
 *   size_t n: The number of bytes to compare.
 * Returns (see above)
 */
int strncmp(const char *s1, const char *s2, size_t n);


/*
 * Copies a string.
 * Arguments:
 *   char *dest: The new string.
 *   const char *src: The source string.
 *   size_t n: The number of bytes to copy.
 * Returns:
 *   A pointer to the new string.
 */
char* strncpy(char *dest, const char *src, size_t n);


/*
 * Calculates the length of a string.
 * Argument:
 *   const char *string: The string to find the length of.
 * Returns:
 *   The length of string.
 */
size_t strlen(const char *string);


/*
 * Calculates the length of a string, up to a given size.
 * Argument:
 *   const char *string: The string to find the length of.
 *   size_t n: The counting limit
 * Returns:
 *   The length of string.
 */
size_t strnlen(const char *string, size_t n);


/*
 * Converts the alphabetical characters
 * in a string to lowercase.
 * Argument:
 *   char *string: The string to be modified.
 */
void strlower(char *string);


/*
 * Converts the alphabetical characters
 * in a string to uppercase.
 * Argument:
 *   char *string: The string to be modified.
 */
void strupper(char *string);

#endif
