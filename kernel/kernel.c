/* Kernel Headers */
#include <drivers/console.h>
#include <kernel/string.h>
#include <x86/x86.h>

#ifdef __file
    #define file "["__file"]: "
    #define file_l 12
#endif

/* Linker Script Symbol - Kernel size string */
extern char* kernel_size;

/*
 * The main kernel function; this is where Ritchie begins operation.
 * The system will halt when/if this function returns.
 */
void kernel(void) {
    console_clear();

    __console_write(file, file_l);
    console_printf(FG_GREEN, "Kernel started\n");

    /* Get Kernel size and CPU vendor id */
    char* size = itoa(((uint32_t) &kernel_size) / 1024);
    char id[13] = {0};
    cpuid_vendor(id);

    __console_write(file, file_l);
    console_printf(FG_WHITE, "Vendor_id: %s\n", id);

    __console_write(file, file_l);
    console_printf(FG_WHITE, "Kernel size: %sKB\n", size);

    /* Initialize the Global Descriptor Table */
    gdt_init();

    __console_write(file, file_l);
    console_write("System halted", 13, FG_RED);
    return; /* Halt the system */
}
