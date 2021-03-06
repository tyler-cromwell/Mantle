########################################################################
#  Copyright (C) 2015-Present Tyler Cromwell <tjc6185@gmail.com>
#
#  This file is part of Mantle.
#
#  Mantle is free software: you can redistribute it and/or modify
#  it under Version 2 of the terms of the GNU General Public License
#  as published by the Free Software Foundation.
#
#  Mantle is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY of FITNESS FOR A PARTICULAR PURPOSE. See the 
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with Mantle.
#  If not, see <http://www.gnu.org/licenses/old-licenses/gpl-2.0.html>
########################################################################

# Important variables
NAME = <NAME>
VERSION = <VERSION>
CODENAME = <CODENAME>
IMAGE = <IMAGE>
STRING = <STRING>
LD_SCRIPT = link.ld

# Programs for building
AS = nasm
CC = ~/workspace/x86_64-elf-5.3.0-Linux-x86_64/bin/x86_64-elf-gcc
LD = ~/workspace/x86_64-elf-5.3.0-Linux-x86_64/bin/x86_64-elf-gcc

MACROS = -D NAME=\"$(NAME)\" -D VERSION=\"$(VERSION)\" -D CODENAME=\"$(CODENAME)\" -D STRING=\"$(STRING)\"
INCLUDE = -I ./include/ -I ./arch/include/

ASFLAGS = -felf64
CFLAGS = -ffreestanding -mcmodel=large -mno-red-zone -mno-mmx -mno-sse -mno-sse2 -std=gnu99
LDFLAGS = -ffreestanding -nostdlib -lgcc -z max-page-size=0x1000 -T $(LD_SCRIPT)

ASM_SRC = $(shell find ./ -name '*.asm')
ASM_OBJ = $(ASM_SRC:%.asm=%.o)
ASM_OBJ_DBG = $(ASM_SRC:%.asm=%.o.debug)

C_SRC = $(shell find ./ -name '*.c')
C_OBJ = $(C_SRC:%.c=%.o)
C_OBJ_DBG = $(C_SRC:%.c=%.o.debug)

%.o: %.asm
	$(AS) $(ASFLAGS) $< -o $@

%.o: %.c
	$(CC) $(CFLAGS) $(MACROS) $(INCLUDE) -c $< -o $@

%.o.debug: %.asm
	$(AS) $(ASFLAGS) -F dwarf -g $< -o $@

%.o.debug: %.c
	$(CC) $(CFLAGS) -ggdb $(MACROS) -D DEBUG $(INCLUDE) -c $< -o $@

.PHONY: all
all: $(ASM_OBJ) $(C_OBJ)
	$(LD) $(LDFLAGS) -o $(IMAGE) $(C_OBJ) $(ASM_OBJ)

.PHONY: debug
debug: $(ASM_OBJ_DBG) $(C_OBJ_DBG)
	$(LD) $(LDFLAGS) -o $(IMAGE).debug $(C_OBJ_DBG) $(ASM_OBJ_DBG)

.PHONY: iso
iso: all
	@rm -rf isodir/
	@mkdir -p isodir
	@mkdir -p isodir/boot
	@cp $(IMAGE) isodir/boot/$(IMAGE)
	@mkdir -p isodir/boot/grub
	@cp grub.cfg isodir/boot/grub/grub.cfg
	grub-mkrescue -o $(IMAGE).iso isodir

.PHONY: clean
clean:
	@find ./ -name '*.o' | xargs rm -rf
	@find ./ -name '*.o.debug' | xargs rm -rf
	@rm -rf $(IMAGE)
	@rm -rf $(IMAGE).debug
	@rm -rf $(IMAGE).iso
	@rm -rf isodir/
