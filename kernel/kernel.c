/* Kernel Headers */
#include <drivers/console.h>
#include <kernel/kernel.h>
#include <kernel/string.h>
#include <x86/multiboot.h>
#include <x86/x86.h>

#ifdef __file
    #define file "["__file"]: "
    #define file_l 12
#endif

#define VENDOR_INTEL    "GenuineIntel"
#define VENDOR_AMD      "AuthenticAMD"

/* Linker Script Symbol - Kernel size string */
extern char* kernel_size;

/*
 * The main kernel function; this is where Ritchie begins operation.
 * The system will halt when/if this function returns.
 * Arguments:
 *   uint32_t magic: A Multiboot bootloaders magic number.
 *   struct MultibootInfo* mbinfo:
 *       The physical memory address of the Multiboot information struct.
 */
void kernel(uint32_t magic, struct MultibootInfo* mbinfo) {
    console_clear();

    debug_console_write(file, file_l);
    console_printf(FG_GREEN, "Kernel Started\n");
    debug_console_write(file, file_l);
    console_printf(FG_BLUE_L, STRING"\n");

    /* Initialize the Global Descriptor Table */
    gdt_init();

    /* Get Kernel size and CPU vendor id */
    uint32_t size = ((uint32_t) &kernel_size) / 1024;
    char id[13] = {0};
    cpuid_vendor(id);

    debug_console_write(file, file_l);
    console_printf(FG_WHITE, "Vendor_id: ");
    if (!strncmp(id, VENDOR_INTEL, strlen(id)))
        console_printf(FG_CYAN_L, "%s\n", id);
    else if (!strncmp(id, VENDOR_AMD, strlen(id)))
        console_printf(FG_RED_L, "%s\n", id);

    debug_console_write(file, file_l);
    console_printf(FG_WHITE, "Kernel size: %uKB\n", size);

    /* Was the kernel booted by a Multiboot bootloader? */
    if (magic == MULTIBOOT_BOOT_MAGIC) {
        multiboot_init(mbinfo);
        multiboot_dump();
    }

    debug_console_write(file, file_l);
    console_printf(FG_RED, "System halted");
    return; /* Halt the system */
}
