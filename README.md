# Ritchie

## What is Ritchie?
Ritchie is a hobbyist Operating System Kernel I am working on to learn more about different CPU architectures (currently x86/amd64), Operating System concepts, and to get more C/Assembly programming practice.<br/>
<br/>
This project is named in honor of the late, great [Dennis Ritchie][dmr].

## Repository Hierarchy
- `arch` - Architecture specific code.
- `drivers` - Driver code.
- `include` - Platform independent header files.
- `kernel` - Kernel core.

## Toolchain
- `Makefile` - This Makefile is used to build the kernel and test it. Rules are as follows:
  - `make` - Builds the kernel image file.
  - `make qemu` - Boots the kernel under QEMU.
  - `make clean` - Deletes all object files and the kernel image file.
- `link.ld` - Simple linker script that determines the physical address the kernel is loaded at during boot.

[dmr]: http://en.wikipedia.org/wiki/Dennis_Ritchie
