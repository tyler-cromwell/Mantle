########################################################################
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
########################################################################

# Important variables
include make.conf
LD_SCRIPT = link.ld

# Programs for building
AS = nasm
CC = x86_64-elf-gcc
LD = x86_64-elf-gcc

INCLUDE = -I ./include/

ASFLAGS = -felf64
CFLAGS = -ffreestanding -mcmodel=large -mno-red-zone -mno-mmx -mno-sse \
		 -mno-sse2 -std=gnu99 $(INCLUDE)
LDFLAGS = -ffreestanding -nostdlib -lgcc -z max-page-size=0x1000 \
		  -T $(LD_SCRIPT) -o $(IMAGE)

ASM_SRC = $(shell find ./ -name '*.asm')
ASM_OBJ = $(ASM_SRC:%.asm=%.o)

C_SRC = $(shell find ./ -name '*.c')
C_OBJ = $(C_SRC:%.c=%.o)

%.o: %.asm
	$(AS) $(ASFLAGS) $< -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: all
all: $(C_OBJ) $(ASM_OBJ)
	$(LD) $(LDFLAGS) $(C_OBJ) $(ASM_OBJ)

.PHONY: iso
iso: all
	@rm -rf isodir/
	@mkdir -p isodir
	@mkdir -p isodir/boot
	@cp $(IMAGE) isodir/boot/$(IMAGE)
	@mkdir -p isodir/boot/grub
	@cp grub.cfg isodir/boot/grub/grub.cfg
	grub2-mkrescue -o $(IMAGE).iso isodir

.PHONY: clean
clean:
	@find ./ -name '*.o' | xargs rm -rf
	@rm -rf $(IMAGE)
	@rm -rf $(IMAGE).iso
	@rm -rf isodir/
