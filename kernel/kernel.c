/* Kernel Headers */
#include <drivers/console.h>
#include <kernel/string.h>
#include <x86.h>

#ifdef _FILE_
#define file "["_FILE_"]: "
#define file_l 12
#endif

/* Linker Script Symbol */
extern char kernel_size[];

/*
 * The main kernel function; this is where Humboldt begins operation.
 * The system will halt when/if this function returns.
 */
void kernel(void) {
    #ifdef _FILE_
    console_write(file, file_l, FG_GREY_L);
    #endif
    console_write("Kernel started\n", 15, FG_GREEN);

    char* size = itoa(((uint32_t) &kernel_size) / 1024);

    char id[14] = {0};
    id[12] = '\n';
    cpuid_vendor(id);

    #ifdef _FILE_
    console_write(file, file_l, FG_GREY_L);
    #endif
    console_write("Kernel size: ", 13, FG_WHITE);
    console_write(size, 20, FG_WHITE);
    console_write("KB\n", 4, FG_WHITE);

    #ifdef _FILE_
    console_write(file, file_l, FG_GREY_L);
    #endif
    console_write("Vendor_id: ", 12, FG_WHITE);
    console_write(id, 14, FG_WHITE);

    gdt_init();

    #ifdef _FILE_
    console_write(file, file_l, FG_GREY_L);
    #endif
    console_write("System halted\n", 14, FG_RED);
    return; /* Halt the system */
}
