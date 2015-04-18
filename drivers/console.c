/* C Standard Library Headers,
   these don't need to link against libc */
#include <stddef.h>
#include <stdint.h>

/* Kernel Headers */
#include <kernel/mem.h>

#define VGA_START   (char*) 0xb8000 /* The starting address for video memory */
#define VGA_END     (char*) 0xb8fa0 /* The ending address for video memory */

#define CHAR_WIDTH  2       /* The number of bytes used per character */
#define LINES       25      /* The number of lines on the screen */
#define LINE_CHARS  80      /* The number of characters per line */
#define LINE_BYTES  160     /* The number of bytes per line */
#define BYTES       4000    /* The number of usable bytes in video memory */

/* Pointer to the next "empty" character byte */
static char* next = VGA_START;

/*
 * Clears the console by zero-ing the screen buffer.
 * Side Effect:
 *   Resets the video pointer.
 */
void console_clear(void) {
    next = VGA_START;
    memset(VGA_START, 0, BYTES);
}

/*
 * Sets the background to a specific color.
 * Argument:
 *   uint8_t attribute: The color attribute.
 */
void console_set_background(uint8_t attribute) {
    next = VGA_START;
    uint16_t offset = 0;

    while (offset < BYTES) {
        if (attribute >= 0x10) {
            next[offset+1] = (next[offset+1] % 0x10) + attribute;
        }
        offset += CHAR_WIDTH;
    }
}

/*
 * Writes a string of characters to the console.
 * Will interpret the newline character.
 * Arguments:
 *   char* message:  The message to write.
 *   uint16_t length: The number of bytes to write.
 *   uint8_t attribute: The coloring attribute.
 * Returns:
 *   The number of characters written.
 * Side Effect:
 *   Updates the video pointer.
 */
size_t console_write(char* message, size_t length, uint8_t attribute) {
    size_t c = 0;

    /* Ensure that the number of characters to write does not exceed the maximum */
    while (message[c] != '\0' && c < length) {
        if (next >= VGA_END) {
            /* Scroll everything up one row */
            next -= LINE_BYTES;
            char* start = VGA_START;

            for (uint16_t i = LINE_BYTES; i < BYTES; i++) {
                uint16_t j = i - LINE_BYTES;
                start[j] = start[i];
            }

            memset(next, 0, LINE_BYTES);
        }

        /* Interpret the newline character */
        if (message[c] == '\n') {
            uint16_t remaining = LINE_CHARS - (((next - VGA_START) % LINE_BYTES) / CHAR_WIDTH);

            /* Zero-out the rest of the line */
            for (uint8_t i = 0; i < remaining; i++) {
                *next = '\0';
                *(next+1) = *(next+1) | (attribute % 0x10);
                next += CHAR_WIDTH;
            }
        }
        else {
            *next = message[c];
            *(next+1) = *(next+1) | (attribute % 0x10);
            next += CHAR_WIDTH;
        }
        c++;
    }

    return c;
}
