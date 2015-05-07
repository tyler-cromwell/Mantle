/* C Standard Library Headers,
   these don't need to link against libc */
#include <stddef.h>

/* Kernel Headers */
#include <drivers/console.h>
#include <kernel/mem.h>
#include <kernel/string.h>
#include <x86/multiboot.h>

#ifdef __file
    #define file "["__file"]: "
    #define file_l 15
#endif

/* Macros for converting byte metrics */
#define CONVERT_NUM 1024
#define CONVERT_UP(number) (number) / CONVERT_NUM
#define CONVERT_DOWN(number) (number) * CONVERT_NUM

#define MMAP_AVAILABLE      1           /* Memory available for use */
#define MMAP_KERNEL_LENGTH  16777216    /* "Allocated" length for the kernel binary (16MB) */

/* Multiboot information structures */
static struct MultibootInfo* info;
static struct MultibootMmap* mmap;

/*
 * Initializes all applicable structures from information passed in from the Bootloader.
 * Argument(s):
 *   struct MultibootInfo* mbinfo: Pointer to the info struct created by the Bootloader.
 */
void multiboot_init(struct MultibootInfo* mbinfo) {
    info = mbinfo;

    /* Initialize the Memory Map */
    if (info->flags & MULTIBOOT_MMAP) {
        mmap = (struct MultibootMmap*) info->mmap_addr;
        size_t ents = info->mmap_length / sizeof(struct MultibootMmap);

        for (size_t i = 0; i < ents; i++) {
            /* If the kernel region fits in the first available region */
            if (mmap[i].type == MMAP_AVAILABLE &&
                mmap[i].length >= MMAP_KERNEL_LENGTH) {

                /* Create a Memory hole */
                mmap[i].base_addr += MMAP_KERNEL_LENGTH;
                mmap[i].length -= MMAP_KERNEL_LENGTH;
                break;
            }
        }
    }
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
