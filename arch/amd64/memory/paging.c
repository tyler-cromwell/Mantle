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

// C Standard Library header(s)
#include <stddef.h>

// Kernel header(s)
#include <amd64/console.h>
#include <kernel/types.h>

// Macro Constants
#define MAX_PDP     (512) - 1
#define MAX_PD      (512 * 512) - 1
#define MAX_PT      (512 * 512 * 512) - 1
#define MAX_PAGE    (512 * 512 * 512 * 512) - 1

// Counters
static ushort_t pointers = 0;
static uint_t directories = 0;
static uint_t tables = 0;
static ulong_t pages = 0;
static ulong_t unmapped_frames = 0;


/*
 * Initializes the paging tree.
 *
 * Only called in "init.c"
 *
 * Arguments:
 *   size_t memory: Size of installed memory (KB).
 *   ulong_t page_num: Total number of in-use pages.
 */
void paging_configure(size_t memory, ulong_t page_num) {
    size_t max_frames = memory / 4;     // 4KB per page/frame

    // Setup counters
    pages = page_num;
    tables = (pages + 512 - (pages % 512)) / 512;
    directories = (tables + 512 - (tables % 512)) / 512;
    pointers = (directories + 512 - (directories % 512)) / 512;
    unmapped_frames = max_frames - pages;

    console_printf(FG_WHITE, "Paging setup\n");
}


/*
 * Prints some basic info about the Paging system.
 *
 * Only called in "init.c"
 */
void paging_pageinfo(void) {
    console_printf(FG_WHITE, "Unmapped frames:\t%u\n", unmapped_frames);
    console_printf(FG_WHITE, "Pages:\t\t\t\t%u\n", pages);
    console_printf(FG_WHITE, "Tables:\t\t\t\t%u\n", tables);
    console_printf(FG_WHITE, "Directories:\t\t%u\n", directories);
    console_printf(FG_WHITE, "Pointers:\t\t\t%u\n", pointers);
}
