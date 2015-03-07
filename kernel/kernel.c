/* Kernel Headers */
#include <drivers/console.h>
#include <lib/string.h>
#include <x86/cpuid.h>

/*
 * The main kernel function; this is where Humboldt begins operation.
 * The system will halt when/if this function returns.
 */
void kernel(void) {
    char* file = "["_FILE_"]: ";
    char* started = "Kernel started";
    char* halted = "System halted";
    char* field = "Vendor_id: ";

    char id[13];
    cpuid_vendor(id);

    console_write(file, strlen(file), FG_GREY_L);
    console_write_line(started, strlen(started), FG_GREEN);

    console_write(file, strlen(file), FG_GREY_L);
    console_write(field, strlen(field), FG_WHITE);
    console_write_line(id, 13, FG_WHITE);

    console_write(file, strlen(file), FG_GREY_L);
    console_write_line(halted, strlen(halted), FG_RED);

    return; /* Halt the system (the 'hlt' instruction is next) */
}
