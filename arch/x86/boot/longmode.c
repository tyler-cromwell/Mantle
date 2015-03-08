#include <drivers/console.h>
#include <lib/string.h>

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
        char* mesg = "Long mode is supported";
        size_t mesg_l = strlen(mesg);

        console_write_line(mesg, mesg_l, FG_BROWN_L);
    }
    else {
        char* mesg = "Long mode is NOT supported, halting...";
        size_t mesg_l = strlen(mesg);

        console_write_line(mesg, mesg_l, FG_RED);
        __asm__("hlt");
    }
}

/*
 * Disables 32-bit Protected mode Paging.
 */
void disable_paging(void) {
    char* mesg = "Paging disabled";
    size_t mesg_l = strlen(mesg);

    __asm__("mov %eax, %cr0");
    __asm__("and %eax, 0x7fffffff");
    __asm__("mov %cr0, %eax");

    console_write(file, file_l, FG_GREY_L);
    console_write_line(mesg, mesg_l, FG_BROWN_L);
}
