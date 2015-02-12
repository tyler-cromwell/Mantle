/* C Standard Library Headers,
   these don't need to link against libc */
#include <stddef.h>
#include <stdint.h>

#define VGA_START   (char*) 0xb8000 /* The starting address for video memory */
#define VGA_END     (char*) 0xb8fa0 /* The ending address for video memory */

#define CHAR_WIDTH  2       /* The number of bytes used per character */
#define LINES       25      /* The number of lines on the screen */
#define LINE_CHARS  80      /* The number of characters per line */
#define LINE_BYTES  160     /* The number of bytes per line */
#define BYTES       4000    /* The number of usable bytes in video memory */

/* Pointer to the next "empty" character byte */
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
        vga[b] = '\0';
        vga[b+1] = '\0';
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
size_t console_write(char* message, size_t length, uint8_t attribute) {
    char* vga = VGA_NEXT;
    size_t c = 0;

    /* Ensure that the number of characters to write does not exceed the maximum */
    while (message[c] != '\0') {
        if (vga >= VGA_END) {
            /* Scroll everything up one row */
            vga -= LINE_BYTES;
            char* start = VGA_START;

            for (uint16_t i = LINE_BYTES; i < BYTES; i++) {
                uint16_t j = i - LINE_BYTES;
                start[j] = start[i];
            }
            
            for (uint8_t i = 0; i < LINE_BYTES; i++) vga[i] = '\0';
        }

        *vga = message[c];
        *(vga+1) = attribute;
        c++;
        vga += CHAR_WIDTH;
    }

    VGA_NEXT = vga;
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
size_t console_write_line(char* message, size_t length, uint8_t attribute) {
    size_t written = console_write(message, length, attribute);
    uint16_t remaining = LINE_CHARS - (((VGA_NEXT - VGA_START) % LINE_BYTES) / CHAR_WIDTH);
    char* vga = VGA_NEXT;

    for (uint8_t i = 0; i < remaining; i++) {
        *vga = '\0';
        *(vga+1) = '\0';
        vga += CHAR_WIDTH;
    }

    VGA_NEXT = vga;
    return written;
}
