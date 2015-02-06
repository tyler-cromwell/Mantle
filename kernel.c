#include <stdint.h>

#include <console.h>

void kernel_init(void) {
    console_clear();
    console_write_newline("Kernel started!", 16, FG_BROWN);

    console_write("System Halted!", 14, FG_BROWN);
    return; // the 'hlt' instruction is next
}
