OUTPUT_FORMAT(elf32-i386)
ENTRY(start)
SECTIONS
{
    . = 0x100000; /* The address to load the Kernel */
    .text : { *(.text) }
    .data : { *(.data) }
    .bss  : { *(.bss)  }
}
