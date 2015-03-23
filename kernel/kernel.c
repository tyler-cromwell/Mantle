/* Kernel Headers */
#include <drivers/console.h>
#include <kernel/string.h>
#include <x86/cpuid.h>
#include <x86/gdt.h>

static char* file = "["_FILE_"]: ";
static size_t file_l = 12;

/*
 * The main kernel function; this is where Humboldt begins operation.
 * The system will halt when/if this function returns.
 */
void kernel(void) {
    gdt_init();

    console_write(file, file_l, FG_GREY_L);
    console_write("Kernel started\n", 15, FG_GREEN);

    char id[14] = {0};
    id[12] = '\n';
    cpuid_vendor(id);

    char* field = "Vendor_id: ";
    console_write(file, file_l, FG_GREY_L);
    console_write(field, strlen(field), FG_WHITE);
    console_write(id, 14, FG_WHITE);

    console_write(file, file_l, FG_GREY_L);
    console_write("System halted\n", 14, FG_RED);
    return; /* Halt the system */
}
