#include "global.h"

extern s32 get_random_range(s32);
extern void *play_sound(struct SongHeader *);

void func_0801A688(void) {
    register u8 *base asm("r4") = (u8 *)0x083B2040;
    u32 offset;

    asm("" : "+r"(base));
    offset = (u32)get_random_range(2);
    offset = (offset << 0x10) >> 0x0E;
    offset += (u32)base;
    play_sound(*(struct SongHeader **)offset);
}
