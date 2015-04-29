/* C Standard Library Headers,
   these don't need to link against libc */
#include <stddef.h>

/* Kernel Headers */
#include <drivers/console.h> // Delete
#include <kernel/mem.h>
#include <kernel/string.h> // Delete
#include <x86/multiboot.h>

#define MMAP_MAX 20 /* Maximum memory map entries (completely arbitrary for now) */

/* Multiboot information structures */
static struct MultibootInfo* information;
static struct MultibootMemoryRegion mmap[MMAP_MAX];

void multiboot_init(struct MultibootInfo* mbinfo) {
    information = mbinfo;

    if (information->flags & MULTIBOOT_MMAP) {
        /* Partition the memory map buffer into an array of structs */
        memcpy(mmap, (void*) information->mmap_addr, information->mmap_length);
        
        size_t ents = information->mmap_length / sizeof(struct MultibootMemoryRegion);

        // TODO: Add an entry for the 16MB Kernel region
        console_printf(FG_CYAN, "Memory Map:\n0");
        for (size_t i = 0; i < ents; i++) {
            char* l = itoa(mmap[i].base_addr / 1024);
            if (strlen(l) >= 4) {
                l = itoa(mmap[i].base_addr / 1024 / 1024);
                console_printf(FG_CYAN, "%sMB - ", l);
            }
            else console_printf(FG_CYAN, "%sKB - ", l);

            char* h = itoa((mmap[i].base_addr + mmap[i].length) / 1024);
            if (strlen(h) >= 4) {
                h = itoa((mmap[i].base_addr + mmap[i].length) / 1024 / 1024);
                console_printf(FG_CYAN, "%sMB (", h);
            }
            else console_printf(FG_CYAN, "%sKB (", h);

            char* len = itoa(mmap[i].length / 1024);
            console_printf(FG_CYAN, "%sKB, ", len);
            
            if (mmap[i].type == MULTIBOOT_MMAP_AVAILABLE)
                console_printf(FG_CYAN, "Available");
            else if (mmap[i].type == MULTIBOOT_MMAP_RESERVED)
                console_printf(FG_CYAN, "Reserved");
            else
                console_printf(FG_CYAN, "?");

            console_printf(FG_CYAN, ")\n");
        }
    }
    if (information->flags & MULTIBOOT_DRIVES) {}
}
