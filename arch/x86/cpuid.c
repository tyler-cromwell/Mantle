/* Kernel Headers */
#include <drivers/console.h>

/*
 * Obtains the CPU vendor string using CPUID.
 * Argument:
 *   char* id: Pointer to the location to save the vendor id.
 */
void cpuid_vendor(char* id) {
    uint32_t eax = 0, ebx = 0, ecx = 0, edx = 0;

    /* Pass 'eax' into register A, then run CPUID */
    __asm__("cpuid": : "a" (eax));

    /* Get the value of registers B, C, and D */
    __asm__("": "=b" (ebx));
    __asm__("": "=c" (ecx));
    __asm__("": "=d" (edx));
    
    /* Get the vendor id of the CPU */
    for (uint8_t i = 0; i < 4; i++) {
        id[i+0] = ebx >> (i * 8);
        id[i+4] = edx >> (i * 8);
        id[i+8] = ecx >> (i * 8);
    }
}
