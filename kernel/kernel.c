/* Kernel Headers */
#include <drivers/console.h>
#include <kernel/string.h>
#include <x86/x86.h>

#ifdef __file
    #define file "["__file"]: "
    #define file_l 12
#endif

/* Linker Script Symbol */
extern char* kernel_size;

/*
 * The main kernel function; this is where Ritchie begins operation.
 * The system will halt when/if this function returns.
 */
void kernel(void) {
    console_clear();

    __console_write(file, file_l);
    console_write("Kernel started\n", 15, FG_GREEN);

    char* size = itoa(((uint32_t) &kernel_size) / 1024);

    char id[13] = {0};
    id[12] = '\n';
    cpuid_vendor(id);

    __console_write(file, file_l);
    console_write("Kernel size: ", 13, FG_WHITE);
    console_write(size, 2, FG_WHITE);
    console_write("KB\n", 3, FG_WHITE);

    __console_write(file, file_l);
    console_write("Vendor_id: ", 11, FG_WHITE);
    console_write(id, 13, FG_WHITE);

    gdt_init();

    __console_write(file, file_l);
    console_write("System halted", 13, FG_RED);
    return; /* Halt the system */
}
