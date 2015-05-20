#  Copyright (C) 2015 Tyler Cromwell <tyler@csh.rit.edu>
#
#  This file is part of Ritchie.
#
#  Ritchie is free software: you can redistribute it and/or modify
#  it under Version 2 of the terms of the GNU General Public License
#  as published by the Free Software Foundation.
#
#  Ritchie is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY of FITNESS FOR A PARTICULAR PURPOSE. See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with Ritchie.
#  If not, see <http://www.gnu.org/licenses/old-licenses/gpl-2.0.html>

# Programs for building
CC = clang

# Respective flags
CPPFLAGS = -I $(ROOT)/include -I $(ROOT)/arch/include
CFLAGS = -nostdlib -Wall -Werror -Wno-unused-command-line-argument -pedantic -m32 -O0

# Debugging options
CPPFLAGS +=

# C source files
C_SRC = $(shell ls *.c)
C_OBJ_X86 = $(C_SRC:%.c=%.o)

# Rule for compiling .c files
%.o: %.c
	$(CC) $(CPPFLAGS) -D ARCH_X86 $(CFLAGS) -c $< -o $@

.PHONY: all
all:
ifeq ($(ARCH),x86)
	$(MAKE) x86
endif

.PHONY: x86
x86: $(C_OBJ_X86)
