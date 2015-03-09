/* Kernel Headers */
#include <drivers/console.h>
#include <kernel/string.h>
#include <x86/cpuid.h>

static char* file = "["_FILE_"]: ";
static size_t file_l = 12;

/*
 * The main kernel function; this is where Humboldt begins operation.
 * The system will halt when/if this function returns.
 */
void kernel(void) {
    char* started = "Kernel started";
    char* halted = "System halted";
    char* field = "Vendor_id: ";

    char id[13] = {0};
    cpuid_vendor(id);

    console_write(file, file_l, FG_GREY_L);
    console_write_line(started, strlen(started), FG_GREEN);

    console_write(file, file_l, FG_GREY_L);
    console_write(field, strlen(field), FG_WHITE);
    console_write_line(id, 13, FG_WHITE);

    console_write(file, file_l, FG_GREY_L);
    console_write_line(halted, strlen(halted), FG_RED);

    return; /* Halt the system */
}
