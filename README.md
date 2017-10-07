# Ritchie

## What is Ritchie?
Ritchie is a hobbyist Operating System Kernel I am working on to learn more about different CPU architectures (currently x86/amd64), Operating System concepts, and to get more C/Assembly programming practice.
I also hope that this project will serve as a good learning reference for others interested in OS development.<br/>
<br/>
This project is named in honor of the late, great [Dennis Ritchie][dmr].

## Repository Hierarchy
- `arch` - Architecture specific code.
    - `arch/include` - Architecture specific header files.
- `docs` - Documentation.
- `include` - Architecture independent headers.
- `lib` - Platform independent library functions.

## Toolchain
- Linux x86_64 GCC Cross Compiler - Download [here][cross].
- `Makefile` - [GNU] Makefile used to build the kernel.
  - `[g]make` - Builds the kernel image file.
  - `[g]make iso` - Builds an iso image with GRUB as the bootloader.
  - `[g]make clean` - Deletes all object files and the kernel image/iso file(s).
- `link.ld` - Linker script that organizes the binary layout.
- `grub.cfg` - Basic GRUB config file.
- `configure.py` - Python 3 script that updates the following files:
  - `make.conf`, `grub.cfg`, and `version.h`.
  - Running `./configure.py -c clean` will run `git checkout` on each file.
- `ritchie.conf` - Project configuration file, contains various options.
  - `Name`: The project name.
  - `Version`: The version string.
  - `Codename`: The codename.
- `make.conf` - File containing Makefile variables (populated by `configure.py`).
  - `ARCH`: The architecture to build.
  - `IMAGE`: The kernel binary image.

## Dependencies
- GCC Cross Compiler
- Python 3
- NASM
- grub2
- xorriso (>= 1.2.9)

## Building / Testing
1. `./configure.py -a amd64`
2. `make iso`
3. Either burn to USB or `qemu-system-x86_64 -cdrom <ISO> -m 2G`

## Branches
- `master` - Mainline branch. Documentation, and feature/subsystem merges are committed to this branch.
- `<branch>` - Branches meant for development of specific features/subsystems (i.e. `paging`).

## Coding Convention
- All uppercase for Macros and Constants with underscores for spaces (i.e. `FG_WHITE`).
- Camelcase for Structs and Struct Typedefs (i.e. `IdtDescriptor`).
- All lowercase, underscores for spaces, and subsystem prefix for function names (i.e. `cpuid_vendor`).

## Editing
For syntax highlighting of the types in `kernel/types.h`, create a file called `~/.vim/after/syntax/c.vim` and add the following lines:
- `syn keyword cType uchar_t ushort_t uint_t ulong_t`
- `syn keyword cType byte_t word_t dword_t qword_t`

[cross]: http://newos.org/toolchains/x86_64-elf-5.3.0-Linux-x86_64.tar.xz
[dmr]: http://en.wikipedia.org/wiki/Dennis_Ritchie
