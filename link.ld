OUTPUT_FORMAT(elf32-i386)
ENTRY(start)
SECTIONS
{
    . = 0x01000000; /* The address to load the Kernel (16MB in) */
    .text : { *(.text) }
    .data : { *(.data) }
    .bss  : { *(.bss)  }
}
