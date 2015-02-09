/* Kernel Headers */
#include <console.h>

void kernel_init(void) {
    console_clear();
    console_write_line("Kernel started!", 16, FG_BROWN);

    console_write_line("System Halted!", 14, FG_BROWN);
    return; // the 'hlt' instruction is next
}
