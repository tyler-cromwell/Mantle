/* C Standard Library Headers,
   these don't need to link against libc */
#include <stdint.h>

/* Kernel Headers */
#include <drivers/console.h>
#include <x86/gdt.h>

#ifdef _FILE_
#define file "["_FILE_"]: "
#define file_l 9
#endif

/* (Information taken from osdev.org)
 * Access Byte:
 * | 7  | 6   5 | 4 | 3  | 2  | 1  | 0  |
 * +----+-------+---+----+----+----+----+
 * | Pr | Privl | 1 | Ex | DC | RW | Ac |
 * +----+-------+---+----+----+----+----+
 *
 * Pr: Present bit, must be one for all valid selectors.
 * Privl: Privilege bits (2). Contains the ring level, 0 = Kernel, 3 = User.
 * Ex: Executable bit. 1 for code selector, 0 for data selector.
 * DC: Direction/Conforming bit.
 *     Direction for data selectors:
 *         0 the segment grows up, 1 the segment grows up.
 *     Conforming for code selectors:
 *         1 if code can be executed from <= privilege level.
 *         0 if only the level set in Privl.
 * RW: Read/Write bit.
 *     For code selectors: Readable bit. Write is never allowed.
 *     For data selectors: Writable bit. Read is always allowed.
 * Ac: Accessed bit, default 0, CPU sets to 1 when accessed.
 *
 * Flags:
 * | 7  | 6  | 5 | 4 |
 * +----+----+---+---+
 * | Gr | Sz | 0 | 0 |
 * +----+----+---+---+
 *
 * Gr: Granularity Bit.
 *     0 if limit is 1 Byte blocks (block granularity).
 *     1 if limit is in 4KB blocks (page granularity).
 * Sz: Size bit.
 *     0 if 16-bit protected mode.
 *     1 if 32-bit protected mode.
 */
struct gdt_descriptor {
    uint16_t limit;             /* 0:15 */
    uint16_t base_low;          /* 0:15 */
    uint8_t base_mid;           /* 16:23 */
    uint8_t access;             /* 47:40 */
    uint8_t granularity;        /* Limit 16:19, Flags 55:52 */
    uint8_t base_high;          /* 24:31 */
} __attribute__((__packed__));  /* Packed to ensure a size of 8 bytes */

struct gdt_ptr {
    uint16_t limit;
    uint32_t base;
} __attribute__((__packed__));

/* The GDT and a pointer to it */
struct gdt_descriptor gdt[4];
struct gdt_ptr gdtr;

/* External - Flush the GDT */
extern void gdt_flush(struct gdt_ptr);

/*
 * Builds a Global Descriptor Table descriptor and writes it to the given 'index'.
 * The maximum number of entries is 8192.
 * Arguments:
 *   uint32_t index: Index in the GDT to write the entry to.
 *   uint32_t base: Descriptor base address.
 *   uint32_t limit: Descriptor limit.
 *   uint8_t access: Access byte.
 *   uint8_t gran: Granularity byte, contains the 4 'flags' and upper 4 'limit' bits.
 */
static void gdt_write_descriptor(uint32_t index, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran) {
    struct gdt_descriptor descriptor = {
        .limit = limit & 0xffff,
        .base_low = base & 0xffff,
        .base_mid = (base >> 16) & 0xff,
        .access = access,
        .granularity = ((limit >> 16) & 0x0f) | (gran & 0xf0),
        .base_high = (base >> 24) & 0xff
    };

    gdt[index] = descriptor;
}

/*
 * Installs the Global Descriptor Table.
 */
void gdt_init(void) {
    /* Setup pointer */
    gdtr.limit = (sizeof(struct gdt_descriptor) * 4) - 1;
    gdtr.base = (uint32_t) &gdt;

    /* Initialize entries */
    gdt_write_descriptor(0, 0x0, 0x00000000, 0x00, 0x00);   /* Null, 0x00 */
    gdt_write_descriptor(1, 0x0, 0xffffffff, 0x9a, 0xcf);   /* Code, 0x08 */
    gdt_write_descriptor(2, 0x0, 0xffffffff, 0x92, 0xcf);   /* Data, 0x10 */
    gdt_write_descriptor(3, 0x0, 0xffffffff, 0x96, 0xcf);   /* Stack, 0x18 */

    gdt_flush(gdtr);
    #ifdef _FILE_
    console_write(file, file_l, FG_GREY_L);
    #endif
    console_write("GDT Initialized\n", 16, FG_BROWN_L);
}
