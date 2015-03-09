#include <drivers/console.h>
#include <kernel/string.h>

static char* file = "["_FILE_"]: ";
static size_t file_l = 14;

/*
 * Detects the presence of Long mode on the CPU
 */
void detect_long_mode(void) {
    uint32_t eax = 0;

    /* Pass 0x80000000 into register A, then run CPUID */
    __asm__("cpuid": : "a" (0x80000000));

    /* Copy register A into 'eax' */
    __asm__("": "=a" (eax));

    console_write(file, file_l, FG_GREY_L);

    /* Check for Long mode */
    if (0x80000001 >= eax) {
        char* mesg = "Long mode is supported\n";
        size_t mesg_l = strlen(mesg);

        console_write(mesg, mesg_l, FG_BROWN_L);
    }
    else {
        char* mesg = "Long mode is NOT supported, halting...\n";
        size_t mesg_l = strlen(mesg);

        console_write(mesg, mesg_l, FG_RED);
        __asm__("hlt");
    }
}

/*
 * Prints the "Paging disabled" message.
 * Used to reduce assembly code.
 */
void paging_disabled(void) {
    char* mesg = "Paging disabled\n";
    size_t mesg_l = strlen(mesg);

    console_write(file, file_l, FG_GREY_L);
    console_write(mesg, mesg_l, FG_BROWN_L);
}
