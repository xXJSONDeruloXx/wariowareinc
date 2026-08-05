#include "global.h"

extern s32 get_random_range(s32);
extern void *play_sound(struct SongHeader *);
extern u8 D_083AE438[];

void func_0801915C(void) {
    register u8 *base asm("r4") = D_083AE438;
    register u8 *entry asm("r0");
    u32 offset = (u32)get_random_range(3);
    offset = (offset << 0x10) >> 0x0E;
    entry = base + offset;
    play_sound(*(struct SongHeader **)entry);
}
