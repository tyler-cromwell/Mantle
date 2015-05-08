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
 * Dumps Multiboot information to the console
 */
void multiboot_dump(void) {
    /* Dump the Bootloader name */
    if (info->flags & MULTIBOOT_BOOTLOADER) {
        debug_console_write(file, file_l);
        console_printf(FG_WHITE, "Booted via: ");
        console_printf(FG_BROWN_L, "%s\n", info->boot_loader_name);
    }

    /* Dump number of boot modules */
    if (info->flags & MULTIBOOT_MODULES) {
        char* mods = itoa(info->mods_count);
        debug_console_write(file, file_l);
        console_printf(FG_WHITE, "Number of boot modules: %s\n", mods);
    }

    /* Dump the amount of Lower and Upper Memory */
    if (info->flags & MULTIBOOT_MEMORY) {
        char* lower = itoa(info->mem_lower);
        debug_console_write(file, file_l);
        console_printf(FG_WHITE, "Lower Memory: %sKB\n", lower);

        uint32_t n = info->mem_upper;
        char* upper = itoa(n);
            debug_console_write(file, file_l);
        if (n >= 1024) {
            upper = itoa(n / 1024);
            console_printf(FG_WHITE, "Upper Memory: %sMB\n", upper);
        }
        else console_printf(FG_WHITE, "Upper Memory: %sKB\n", upper);
    }

    /* Dump the Memory Map */
    if (info->flags & MULTIBOOT_MMAP) {
        debug_console_write(file, file_l);
        console_printf(FG_WHITE, "Memory Map:\n");

        size_t ents = info->mmap_length / sizeof(struct MultibootMmap);

        for (size_t i = 0; i < ents; i++) {
            debug_console_write(file, file_l);

            /* Region Base Address */
            uint32_t n = CONVERT_UP(mmap[i].base_addr);
            char* l = itoa(n);
            if (n >= CONVERT_NUM) {
                l = itoa(CONVERT_UP(n));
                console_printf(FG_WHITE, "%sMB - ", l);
            }
            else console_printf(FG_WHITE, "%sKB - ", l);

            /* Region Ending Address */
            n = CONVERT_UP(mmap[i].base_addr + mmap[i].length);
            char* h = itoa(n);
            if (n >= CONVERT_NUM) {
                h = itoa(CONVERT_UP(n));
                console_printf(FG_WHITE, "%sMB (", h);
            }
            else console_printf(FG_WHITE, "%sKB (", h);

            /* Region length */
            n = CONVERT_UP(mmap[i].length);
            char* len = itoa(n);
            if (n >= CONVERT_NUM) {
                len = itoa(CONVERT_UP(n));
                console_printf(FG_WHITE, "%sMB, ", len);
            }
            else console_printf(FG_WHITE, "%sKB, ", len);

            /* Region type */
            if (mmap[i].type == MMAP_AVAILABLE)
                console_printf(FG_WHITE, "Available");
            else
                console_printf(FG_WHITE, "Reserved");

            console_printf(FG_WHITE, ")\n");
        }
    }
}
