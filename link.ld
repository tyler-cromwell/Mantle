/* Referenced from osdev.org */
OUTPUT_FORMAT(elf32-i386)
ENTRY(start)
SECTIONS {
    . = 0x01000000; /* The address to load the Kernel (16MB in) */

    .text BLOCK(4K) : ALIGN(4K) { *(.text) }
    .data BLOCK(4K) : ALIGN(4K) { *(.data) }

    .bss BLOCK(4K) : ALIGN(4K) {
        *(COMMON)
        *(.bss)
    }
}
