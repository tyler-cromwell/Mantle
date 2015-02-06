/* C Standard Library Headers,
   these don't need to link against libc */
#include <stdint.h>

/* Kernel Headers */
#include <console.h>

#define VIDEO_MEM_START (char*) 0xb8000

#define CONSOLE_LINES       25
#define CONSOLE_LINE_CHARS  80
#define CONSOLE_CHAR_WIDTH  2
#define CONSOLE_BYTES       (25 * 80 * 2)

/* Pointer to the next "empty" character byte. */
char* VIDEO_MEM_CURRENT = VIDEO_MEM_START;

/*
 * Clears the console.
 * Side Effect:
 *   Updates the video pointer.
 */
void console_clear(void) {
    char* video_mem = VIDEO_MEM_START;
    uint16_t b = 0;

    while (b < CONSOLE_BYTES) {
        video_mem[b] = 0x0;
        video_mem[b+1] = 0x0;
        b += CONSOLE_CHAR_WIDTH; /* Move up 2 bytes */
    }

    VIDEO_MEM_CURRENT = VIDEO_MEM_START;
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
    char* video_mem = VIDEO_MEM_CURRENT;
    uint16_t b = 0, c = 0;

    /* Ensure that the number of characters to write does not exceed the maximum */
    while (c < (CONSOLE_BYTES / 2) && c < length && message[c] != '\0') {
        video_mem[b] = message[c];
        video_mem[b+1] = attribute;
        c++;
        b += CONSOLE_CHAR_WIDTH;
    }

    VIDEO_MEM_CURRENT += b;
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
    char* video_mem = VIDEO_MEM_CURRENT;

    while (b < (80 - written) * CONSOLE_CHAR_WIDTH) {
        video_mem[b] = 0x0;
        video_mem[b+1] = 0x0;
        b += CONSOLE_CHAR_WIDTH;
    }

    VIDEO_MEM_CURRENT += b;
    return written;
}
