#ifndef X86_MULTIBOOT_H
#define X86_MULTIBOOT_H

#include <stdint.h>

/*
 * Information was referenced from the GNU Multiboot Specification
 * GNU Multiboot 0.6.96 is the supported version, currently.
 */

#define MULTIBOOT_BOOT_MAGIC    0x2badb002  /* Bootloader magic number */
#define MULTIBOOT_MEMORY        1 << 0      /* Lower and Upper Memory flag */
#define MULTIBOOT_MODULES       1 << 3      /* Boot modules flag */
#define MULTIBOOT_MMAP          1 << 6      /* Memory map flag */
#define MULTIBOOT_DRIVE         1 << 7      /* Drives flag */
#define MULTIBOOT_BOOTLOADER    1 << 9      /* Bootloader Name flag */

/* Signifies that the Kernel complies with Multiboot */
struct MultibootHeader {
    uint32_t magic;             /* The header identifier, must be 0x1badb002 */
    uint32_t flags;             /* Feature flags, requirements (0-15), optionals (16-31).
                                   Bit 00: 4KB page alignment
                                   Bit 01: Info in 'mem_*' will be included and 'mmap_*' if supported
                                   Bit 02: Video mode info is passed
                                   Bit 16: Fields at offsets from 12-28 are valid (required unless image is ELF) */
    uint32_t checksum;          /* MBZ when added to Magic and Flags */
    uint32_t header_addr;       /* Multiboot header address */
    uint32_t load_addr;         /* Text segment address. Must be <= header_addr */
    uint32_t load_end_addr;     /* End address of Data segment.
                                   If 0, bootloader assumes Text and Data segments occupy whole image */
    uint32_t bss_end_addr;      /* End address of BSS segment. If 0, bootloader assumes not BSS segment */
    uint32_t entry_addr;        /* Kernel entry point address */
    uint32_t mode_type;         /* 0 for linear graphics, 1 for EGA */
    uint32_t width;             /* Number of columns, pixels in graphics mode, 0 in text */
    uint32_t height;            /* Number of lines, pixels in graphics mode, 0 in text */
    uint32_t depth;             /* Bits per pixel in graphics mode, 0 in text */
} __attribute__((__packed__));

/* Information passed to the Kernel by the Bootloader */
struct MultibootInfo {
    uint32_t flags;         /* Only bits 0 to 11 are vaild, bits 04 and 05 are mutually exclusive */
    uint32_t mem_lower;     /* If bit 00: Amount of lower memory */
    uint32_t mem_upper;     /* If bit 00: Amount of upper memory */
    uint32_t boot_device;   /* If bit 01: BIOS disk device the Kernel was loaded from (string) */
    uint32_t cmdline;       /* If bit 02: Physical address of the null-terminated command line to be passed */
    uint32_t mods_count;    /* If bit 03: Number of boot modules loaded */
    uint32_t mods_addr;     /* If bit 03: Physical address of the first module */

    union MultibootSectionHeaders {
        /* Valid if bit 04 is set and bit 05 is not */
        struct MultibootAoutSectionHeader {
            uint32_t tabsize;   /**/
            uint32_t strsize;   /**/
            uint32_t addr;      /**/
            uint32_t reserved;  /* Must be zero */
        } aout_header __attribute__((__packed__));

        /* Valid if bit 05 is set and bit 04 is not */
        struct MultibootElfSectionHeader {
            uint32_t num;   /**/
            uint32_t size;  /**/
            uint32_t addr;  /**/
            uint32_t shndx; /**/
        } elf_header __attribute__((__packed__));
    } section_headers;

    uint32_t mmap_length;       /* If bit 06: Length of the memory map buffer (bytes) */
    uint32_t mmap_addr;         /* If bit 06: Address of the memory map buffer */
    uint32_t drives_length;     /* If bit 07: Total size of drive structures (bytes) */
    uint32_t drives_addr;       /* If bit 07: Physical address of the first drive structure */
    uint32_t config_table;      /* If bit 08: Address of the ROM configuration table */
    uint32_t boot_loader_name;  /* If bit 09: Name of the Bootloader (null-terminated) */
    uint32_t apm_table;         /* If bit 10: IGNORE - Made obsolete by ACPI */
    uint32_t vbe_control_info;  /* If bit 11 & bit 2 in header: Physical address of VBE info from function 00h */
    uint32_t vbe_mode_info;     /* If bit 11 & bit 2 in header: Physical address of VBE info from function 01h */
    uint16_t vbe_mode;          /* If bit 11 & bit 2 in header: Indicates video mode in the format specified in VBE 3.0 */
    uint16_t vbe_interface_seg; /* If bit 11 & bit 2 in header: */
    uint16_t vbe_interface_off; /* If bit 11 & bit 2 in header: */
    uint16_t vbe_interface_len; /* If bit 11 & bit 2 in header: */
} __attribute__((__packed__));

/*
 * Boot Module structure.
 * May be used if multiboot_info.flags bit 3 is set
 */
struct MultibootModules {
    uint32_t mod_start; /**/
    uint32_t mod_end;   /**/
    uint32_t string;    /**/
    uint32_t reserved;  /* Must be zero */
} __attribute__((__packed__));

/* 
 * A region of memory.
 * May be used if mutliboot_info.flags bit 6 is set
 */
struct MultibootMmap {
    uint32_t size;      /* Size of this struct */
    uint64_t base_addr; /* Starting address of the region */
    uint64_t length;    /* Length of the region (in bytes) */
    uint32_t type;      /* Type of region (1 for available, else reserved) */
} __attribute__((__packed__));

/* May be used if mutliboot_info.flags bit 7 is set */
struct MultibootDrive {
    uint32_t  size;                     /* Size this struct */
    uint8_t   drive_number      : 1;    /* BIOS drive number (0 for CHS, 1 for LBA) */
    uint8_t   drive_mode        : 1;    /* Access mode used by bootloader */
    uint8_t   drive_cylinders   : 1;    /* Number of cylinders */
    uint8_t   drive_heads       : 1;    /* Number of heads */
    uint8_t   drive_sectors     : 1;    /* Number of sectors per track */
    uint16_t* drive_ports;              /* Null terminated array */
} __attribute__((__packed__));

void multiboot_init(struct MultibootInfo*);
void multiboot_dump(void);

#endif
