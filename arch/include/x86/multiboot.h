#ifndef X86_MULTIBOOT_H
#define X86_MULTIBOOT_H

#include <stdint.h>

/* Information was referenced from the GNU Multiboot Specification */

#define MULTIBOOT_BOOT_MAGIC    0x2badb002  /* Multiboot compliant bootloader magic number */
#define MULTIBOOT_MMAP          1 << 6      /* Memory map flag */
#define MULTIBOOT_DRIVES        1 << 7      /* Drives flag */

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

struct MultibootInfo {
    uint32_t flags;             /* Bits 4 & 5 are mutually exclusive, stops at bit 11 */
    uint32_t mem_lower;         /* If bit 00: Integer: Amount of lower memory */
    uint32_t mem_upper;         /* If bit 00: Integer: Amount of upper memory */
    uint32_t boot_device;       /* If bit 01: String: BIOS disk device the Kernel was loaded from */
    uint32_t cmdline;           /* If bit 02: Pointer: Physical address of the null-terminated command line to be passed */
    uint32_t mods_count;        /* If bit 03: Integer: Number of boot modules loaded */
    uint32_t mods_addr;         /* If bit 03: Pointer: Physical address of the first module */
    uint32_t num;               /* If bit 05: (ELF Only): */
    uint32_t size;              /* If bit 05: (ELF Only): */
    uint32_t addr;              /* If bit 05: (ELF Only): */
    uint32_t shndx;             /* If bit 05: (ELF Only): */
    uint32_t mmap_length;       /* If bit 06: Integer: Length of the memory map buffer */
    uint32_t mmap_addr;         /* If bit 06: Pointer: Address of the memory map buffer */
    uint32_t drives_length;     /* If bit 07: Integer: Total size of drive structures */
    uint32_t drives_addr;       /* If bit 07: Pointer: Physical address of the first drive structure */
    uint32_t config_table;      /* If bit 08: Pointer: Address of the ROM configuration table */
    uint32_t boot_loader_name;  /* If bit 09: String: Address of the name of the Bootloader (null-terminated) */
    uint32_t apm_table;         /* If bit 10: Pointer: IGNORE - Made obsolete by ACPI */
    uint32_t vbe_control_info;  /* If bit 11 & bit 2 in header: Physical address of VBE info from function 00h */
    uint32_t vbe_mode_info;     /* If bit 11 & bit 2 in header: Physical address of VBE info from function 01h */
    uint16_t vbe_mode;          /* If bit 11 & bit 2 in header: Indicates video mode in the format specified in VBE 3.0 */
    uint16_t vbe_interface_seg; /* If bit 11 & bit 2 in header: */
    uint16_t vbe_interface_off; /* If bit 11 & bit 2 in header: */
    uint16_t vbe_interface_len; /* If bit 11 & bit 2 in header: */
} __attribute__((__packed__));

/* Memory region types */
#define MULTIBOOT_MMAP_AVAILABLE    1
#define MULTIBOOT_MMAP_RESERVED     2

/* 
 * A region of memory.
 * May be used if mutliboot_info.flags bit 6 is set
 */
struct MultibootMemoryRegion {
    uint32_t size;              /* */
    uint64_t base_addr;         /* */
    uint64_t length;            /* */
    uint32_t type;              /* */
} __attribute__((__packed__));

/* May be used if mutliboot_info.flags bit 7 is set */
struct MultibootDrives {
    uint32_t  size;             /* Size of the structure */
    uint8_t   drive_number;     /* */
    uint8_t   drive_mode;       /* */
    uint16_t  drive_cylinders;  /* */
    uint8_t   drive_heads;      /* */
    uint8_t   drive_sectors;    /* */
    uint16_t* drive_ports;      /* Null terminated array */
} __attribute__((__packed__));

void multiboot_init(struct MultibootInfo*);

#endif
