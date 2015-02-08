/* C Standard Library Headers,
   these don't need to link against libc */
#include <stdint.h>

/* Kernel Headers */
#include <console.h>

#define VGA_START   (char*) 0xb8000

#define LINES       25
#define LINE_CHARS  80
#define CHAR_WIDTH  2
#define BYTES       (25 * 80 * 2)

/* Pointer to the next "empty" character byte. */
char* VGA_NEXT = VGA_START;

/*
 * Clears the console.
 * Side Effect:
 *   Resets the video pointer.
 */
void console_clear(void) {
    char* vga = VGA_START;
    uint16_t b = 0;

    while (b < BYTES) {
        vga[b] = 0x0;
        vga[b+1] = 0x0;
        b += CHAR_WIDTH; /* Move up 2 bytes */
    }

    VGA_NEXT = VGA_START;
}

/*
 * Writes a string of characters to the console.
 * Arguments:
 *   char* message:  The message to write.
 *   uint16_t length: The number of bytes to write.
 *   uint8_t attribute: The coloring attribute.
 * Returns:
 *   The number of characters written.
 * Side Effect:
 *   Updates the video pointer.
*/
uint16_t console_write(char* message, uint16_t length, uint8_t attribute) {
    char* vga = VGA_NEXT;
    uint16_t b = 0, c = 0;

    /* Ensure that the number of characters to write does not exceed the maximum */
    while (c < (BYTES / CHAR_WIDTH) && c < length && message[c] != '\0') {
        vga[b] = message[c];
        vga[b+1] = attribute;
        c++;
        b += CHAR_WIDTH;
    }

    VGA_NEXT += b;
    return c;
}

/*
 * Writes a string of characters to the console, then zeros until the end of the line.
 * Arguments:
 *   char* message:  The message to write.
 *   uint16_t length: The number of bytes to write.
 *   uint8_t attribute: The coloring attribute.
 * Returns:
 *   Returns the number of characters written.
 * Side Effect:
 *   Updates the video pointer.
*/
uint16_t console_write_newline(char* message, uint16_t length, uint8_t attribute) {
    uint16_t written = console_write(message, length, attribute);
    uint16_t b = 0;
    char* vga = VGA_NEXT;

    while (b < (LINE_CHARS - written) * CHAR_WIDTH) {
        vga[b] = 0x0;
        vga[b+1] = 0x0;
        b += CHAR_WIDTH;
    }

    VGA_NEXT += b;
    return written;
}
