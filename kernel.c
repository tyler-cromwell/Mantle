/* Kernel Headers */
#include <drivers/console.h>
#include <humboldt/string.h>

void kernel(void) {
    char* started = "Kernel started!";
    char* halted = "System halted!";

    size_t slen = strlen(started);
    size_t hlen = strlen(halted);

    char* s = itoa(34637472);

    console_clear();
    console_write_line(s, 2, FG_GREY_L);
    console_write_line(started, slen, FG_GREEN);

    console_write_line(halted, hlen, FG_RED);
    return; // the 'hlt' instruction is next
}
