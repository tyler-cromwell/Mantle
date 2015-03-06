/* Kernel Headers */
#include <drivers/console.h>
#include <lib/string.h>
#include <x86/cpuid.h>

/*
 * The main kernel function; this is where Humboldt begins operation.
 * The system will halt when/if this function returns.
 */
void kernel(void) {
    char* started = "Kernel started!";
    char* halted = "System halted!";

    size_t slen = strlen(started);
    size_t hlen = strlen(halted);

    console_write_line(started, slen, FG_GREEN);

    char id[13];
    cpuid_vendor(id);

    console_write("Vendor ID: ", 11, FG_WHITE);
    console_write_line(id, 13, FG_WHITE);

    console_write_line(halted, hlen, FG_RED);
    return; /* Halt the system (the 'hlt' instruction is next) */
}
