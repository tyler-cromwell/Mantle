/* C Standard Library Headers,
   these don't need to link against libc */
#include <stddef.h>

/* Kernel Headers */
#include <drivers/console.h> // Delete
#include <kernel/mem.h>
#include <kernel/string.h> // Delete
#include <x86/multiboot.h>

#ifdef __file
    #define file "["__file"]: "
    #define file_l 15
#endif

/* Macros for converting byte metrics */
#define CONVERT_NUM 1024
#define CONVERT_UP(number) (number) / CONVERT_NUM
#define CONVERT_DOWN(number) (number) * CONVERT_NUM

#define MMAP_MAX        30  /* Maximum memory map entries (completely arbitrary for now) */
#define MMAP_AVAILABLE  1   /* Memory available for use */

/* Multiboot information structures */
static struct MultibootInfo* info;
static struct MultibootMmap mmap[MMAP_MAX];

/*
 * Initializes all applicable structures from information passed in from the Bootloader.
 * Argument(s):
 *   struct MultibootInfo* mbinfo: Pointer to the info struct created by the Bootloader.
 */
void multiboot_init(struct MultibootInfo* mbinfo) {
    info = mbinfo;

    /* Initialize the Memory Map */
    if (info->flags & MULTIBOOT_MMAP) {
        /* Partition the memory map buffer into an array of structs */
        memcpy(mmap, (void*) info->mmap_addr, info->mmap_length);
        
        size_t ents = info->mmap_length / sizeof(struct MultibootMmap);

        /* Create an entry for the Kernel */
        struct MultibootMmap kernel = {
            .size = 20,
            .base_addr = 0x00100000,    /* Start 1MB into RAM */
            .length = 16 * 1024 * 1024, /* Region is 16MB wide, gives space to grow */
            .type = 2                   /* Don't overwrite the kernel */
        };

        for (size_t i = 0; i < ents; i++) {
            uint64_t ent_addr_upper = mmap[i].base_addr + mmap[i].length;
            uint64_t kernel_addr_upper = kernel.base_addr + kernel.length;

            /* If the kernel region fits in the first available region */
            if (mmap[i].type == MMAP_AVAILABLE &&
                kernel.base_addr >= mmap[i].base_addr &&
                kernel_addr_upper <= ent_addr_upper) {

                /* Create a Memory hole */
                mmap[i].base_addr += kernel.length;
                mmap[i].length -= kernel.length;

                /* Fill the hole if there's at least one empty space */
                if (ents < MMAP_MAX) {
                    /* Shift entries forward by one struct */
                    for (size_t j = ents-2; j >= i; j--) {
                        mmap[j+1] = mmap[j];
                    }

                    mmap[i] = kernel; /* Add kernel entry */
                }
                break;
            }
        }
    }

    if (info->flags & MULTIBOOT_DRIVES) {}
}

/*
 * Dumps the Memory Map to console
 */
void multiboot_mmap_dump(void) {
    debug_console_write(file, file_l);
    console_printf(FG_CYAN, "Memory Map:\n");

    size_t ents = info->mmap_length / sizeof(struct MultibootMmap);

    for (size_t i = 0; i < ents; i++) {
        debug_console_write(file, file_l);

        uint32_t n = CONVERT_UP(mmap[i].base_addr);
        char* l = itoa(n);
        if (n >= CONVERT_NUM) {
            l = itoa(CONVERT_UP(n));
            console_printf(FG_CYAN, "%sMB - ", l);
        }
        else console_printf(FG_CYAN, "%sKB - ", l);

        n = CONVERT_UP(mmap[i].base_addr + mmap[i].length);
        char* h = itoa(n);
        if (n >= CONVERT_NUM) {
            h = itoa(CONVERT_UP(n));
            console_printf(FG_CYAN, "%sMB (", h);
        }
        else console_printf(FG_CYAN, "%sKB (", h);

        n = CONVERT_UP(mmap[i].length);
        char* len = itoa(n);
        if (n >= CONVERT_NUM) {
            len = itoa(CONVERT_UP(n));
            console_printf(FG_CYAN, "%sMB, ", len);
        }
        else console_printf(FG_CYAN, "%sKB, ", len);

        if (mmap[i].type == MMAP_AVAILABLE)
            console_printf(FG_CYAN, "Available");
        else
            console_printf(FG_CYAN, "Reserved");

        console_printf(FG_CYAN, ")\n");
    }
}
