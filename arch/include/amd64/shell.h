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

#ifndef AMD64_SHELL_H
#define AMD64_SHELL_H

// C Standard Library header(s)
#include <stdint.h>

// Kernel header(s)
#include <amd64/asm.h>
#include <amd64/multiboot.h>
#include <kernel/types.h>

// Macro Constants
#define SHELL_BUFSIZ    16


/*
 * Basic text input function.
 * Argument:
 *   char *prompt: The prompt to display.
 * Returns:
 *   a pointer to the input buffer.
 */
char* shell_readline(char *prompt);


/*
 * Dump information about the Kernel binary.
 */
void shell_cmd_kinfo(void);


/*
 * Dump information about the Processor.
 */
void shell_cmd_cpuinfo(void);


/*
 * Dump Multiboot information (if available).
 * Arguments:
 *   ulong_t magic: Bootloader magic number.
 */
void shell_cmd_multiboot(ulong_t magic);


/*
 * Initiates system shutdown.
 */
void shell_cmd_shutdown();

#endif
