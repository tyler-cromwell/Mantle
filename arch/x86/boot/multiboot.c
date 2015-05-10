/* C Standard Library Headers,
   these don't need to link against libc */
#include <stddef.h>

/* Kernel Headers */
#include <drivers/console.h>
#include <kernel/mem.h>
#include <kernel/string.h>
#include <x86/multiboot.h>

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
        console_printf(FG_WHITE, "Booted via: ");
        console_printf(FG_GREY_L, "%s\n", info->boot_loader_name);
    }

    /* Dump number of boot modules */
    if (info->flags & MULTIBOOT_MODULES) {
        console_printf(FG_WHITE, "Number of boot modules: %u\n", info->mods_count);
    }

    /* Dump the amount of Lower and Upper Memory */
    if (info->flags & MULTIBOOT_MEMORY) {
        console_printf(FG_WHITE, "Lower Memory: %uKB\n", info->mem_lower);

        uint32_t upper = info->mem_upper;
        if (upper >= 1024)
            console_printf(FG_WHITE, "Upper Memory: %uMB\n", (upper / 1024));
        else
            console_printf(FG_WHITE, "Upper Memory: %uKB\n", upper);
    }

    /* Dump the Memory Map */
    if (info->flags & MULTIBOOT_MMAP) {
        console_printf(FG_WHITE, "Memory Map:\n");

        size_t ents = info->mmap_length / sizeof(struct MultibootMmap);

        for (size_t i = 0; i < ents; i++) {
            /* Region Base Address */
            uint32_t n = CONVERT_UP(mmap[i].base_addr);
            if (n >= CONVERT_NUM) {
                n = CONVERT_UP(n);
                console_printf(FG_WHITE, "%uMB - ", n);
            }
            else console_printf(FG_WHITE, "%uKB - ", n);

            /* Region Ending Address */
            n = CONVERT_UP(mmap[i].base_addr + mmap[i].length);
            if (n >= CONVERT_NUM) {
                n = CONVERT_UP(n);
                console_printf(FG_WHITE, "%uMB (", n);
            }
            else console_printf(FG_WHITE, "%uKB (", n);

            /* Region length */
            n = CONVERT_UP(mmap[i].length);
            if (n >= CONVERT_NUM) {
                n = CONVERT_UP(n);
                console_printf(FG_WHITE, "%uMB, ", n);
            }
            else console_printf(FG_WHITE, "%uKB, ", n);

            /* Region type */
            if (mmap[i].type == MMAP_AVAILABLE)
                console_printf(FG_WHITE, "Available");
            else
                console_printf(FG_WHITE, "Reserved");

            console_printf(FG_WHITE, ")\n");
        }
    }
}
