# Humboldt

## What is Humboldt?
Humboldt is a toy Operating System Kernel I am working on to get familiar with Operating System development.
The project is inspired by Linux and BSD, and gets it's name from the Humboldt Penguin (I'm not very original as far as naming).

## Toolchain
- `Makefile` - This Makefile is used to build the kernel and test it. Rules are as follows:
  - `make` - Builds the kernel image file.
  - `make qemu` - Boots the kernel under QEMU.
  - `make clean` - Deletes all object files and the kernel image file.
- `link.ld` - Simple linker script that determines the physical address the kernel is loaded at during boot.
- `kmkdir.pl` - A Perl script that builds a new directory with a skeleton README.md.
