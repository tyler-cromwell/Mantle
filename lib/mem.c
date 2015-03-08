/* C Standard Library Headers,
   these don't need to link against libc */
#include <stddef.h>
#include <stdint.h>

/*
 * Fills the first 'n' bytes of 'dest' with 'c'.
 * Arguments:
 *   void* dest: The region of memory to be over-written.
 *   int c: The value to write with.
 *   size_t n: The number of bytes to write.
 * Returns:
 *   A pointer to 'dest'.
 */
void* memset(void* dest, int c, size_t n) {
    uint8_t* buf = (uint8_t*) dest;
    for (size_t i = 0; i < n; i++)
        buf[i] = (uint8_t) c;
    return dest;
}
