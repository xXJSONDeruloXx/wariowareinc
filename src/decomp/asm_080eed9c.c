#if __INCLUDE_LEVEL__ > 0
#include "global.h"

struct Animation;

__attribute__((naked))
s8 sprite_anim_get_cel_total(struct Animation *anim) {
    asm volatile(
        ".syntax unified\n"
        "push {lr}\n"
        "adds r2, r0, #0\n"     // r2 = anim pointer
        "movs r1, #0\n"        // r1 = cel count = 0
        "ldr r0, [r2]\n"       // r0 = anim->cel
        "cmp r0, #0\n"
        "beq 2f\n"
        "1:\n"                 // loop start
        "adds r0, r1, #1\n"    // r0 = count + 1
        "lsls r0, r0, #0x18\n" // shift left by 24
        "lsrs r1, r0, #0x18\n" // r1 = (u8)(count + 1) - new count
        "lsls r0, r1, #3\n"    // r0 = count * 8 (sizeof Animation)
        "adds r0, r2\n"        // r0 = anim + count*8
        "ldr r0, [r0]\n"       // r0 = anim[count].cel
        "cmp r0, #0\n"
        "bne 1b\n"             // if cel != NULL, continue
        "2:\n"
        "adds r0, r1, #0\n"    // r0 = count (return value)
        "pop {r1}\n"
        "bx r1\n"
        ".short 0x0000\n"      // padding
        ".syntax divided\n"
    );
}
#endif
