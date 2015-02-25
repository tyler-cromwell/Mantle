/* Kernel Headers */
#include <drivers/console.h>
#include <lib/string.h>

void kernel(void) {
    char* started = "System started!";
    char* halted = "System halted!";

    size_t slen = strlen(started);
    size_t hlen = strlen(halted);

    console_clear();
    console_set_background(BG_BLUE);
    console_write_line(started, slen, FG_GREEN);
    console_write_line(halted, hlen, FG_RED);

    return; /* Halt the system (the 'hlt' instruction is next) */
}
