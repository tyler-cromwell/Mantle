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

/* C Standard Library Headers,
   these don't need to link against libc */
#include <stddef.h>
#include <stdint.h>

/*
 * Copies the contents of one buffer to another.
 * Arguments:
 *   void* dest: The destination buffer.
 *   const void*: The source buffer.
 *   size_t n: The number of bytes to copy.
 * Returns:
 *   A pointer to the destination buffer.
 */
void* memcpy(void* dest, const void* src, size_t n) {
    uint8_t* buf = dest;
    for (size_t i = 0; i < n; i++) {
        buf[i] = ((uint8_t*) src)[i];
    }
    return dest;
}

/*
 * Fills the first 'n' bytes of 'dest' with 'c'.
 * Arguments:
 *   void* dest: The region of memory to be over-written.
 *   int c: The value to write with.
 *   size_t n: The number of bytes to write.
 * Returns:
 *   A pointer to the destination buffer.
 */
void* memset(void* dest, int c, size_t n) {
    uint8_t* buf = dest;
    for (size_t i = 0; i < n; i++) {
        buf[i] = (uint8_t) c;
    }
    return dest;
}
