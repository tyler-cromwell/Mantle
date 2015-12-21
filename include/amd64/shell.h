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

#ifndef AMD64_SHELL_H
#define AMD64_SHELL_H

/* C Standard Library Headers,
   these don't need to link against libc */
#include <stdint.h>

/* Kernel Headers */
#include <amd64/asm.h>
#include <amd64/multiboot.h>

/* Macro Constants */
#define SHELL_BUFSIZ    16

/* Function Prototypes */
char* shell_readline(char *prompt);
void shell_cmd_kinfo(void);
void shell_cmd_cpuinfo(void);
void shell_cmd_multiboot(uint64_t magic, struct MultibootInfo *mbinfo);

#endif
