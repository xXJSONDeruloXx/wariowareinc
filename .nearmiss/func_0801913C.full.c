#include "global.h"

extern s32 get_random_range(s32);
extern void scene_set_music(struct SongHeader *);
extern u8 D_083AE430[];

void func_0801913C(void) {
    register u8 *base asm("r4") = D_083AE430;
    register u8 *entry asm("r0");
    u32 offset = (u32)get_random_range(2);
    offset = (offset << 0x10) >> 0x0E;
    entry = base + offset;
    scene_set_music(*(struct SongHeader **)entry);
}
