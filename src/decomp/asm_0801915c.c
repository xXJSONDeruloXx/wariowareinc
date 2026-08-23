#include "global.h"

extern s32 get_random_range(s32);
extern void *play_sound(struct SongHeader *);
extern u8 D_083AE438[];

void func_0801915C(void) {
    u8 *base;
    u32 offset;

    base = D_083AE438;
    offset = (u32)get_random_range(3);
    offset = (offset << 0x10) >> 0x0E;
    offset += (u32)base;
    play_sound(*(struct SongHeader **)offset);
}
