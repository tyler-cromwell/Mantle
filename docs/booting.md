# Booting

### Introduction
Mantle is a [Multiboot][multiboot]-compliant operating system for the [x86_64] architecture.
In short this means certain bootloaders, such as [Grub][grub] (the Linux bootloader, which the rest of this document will refer to), are capable of booting Mantle.
By the time Mantle begins its own booting process, Grub has already elevated the processor into [32-bit Protected Mode][pmode].
Therefore Mantle simply has to switch the processor into [64-bit Long Mode][lmode] in order to complete the boot process and actually enter the kernel.

The remaining boot process is summarized by the following:
1. Define Multiboot header
2. Define GDT64
3. Check for Long Mode
4. Clear free memory
5. Begin Long Mode setup
     1. Setup Paging (see paging.md for more info)
6. Finish Long Mode setup
7. Load GDT and far jump!
8. Reload segment registers
9. Setup arguments and jump into the kernel

[grub]: https://www.gnu.org/software/grub/
[lmode]: https://en.wikipedia.org/wiki/Long_mode
[multiboot]: https://www.gnu.org/software/grub/manual/multiboot/multiboot.html
[pmode]: https://en.wikipedia.org/wiki/Protected_mode
[x86_64]: https://en.wikipedia.org/wiki/X86-64
