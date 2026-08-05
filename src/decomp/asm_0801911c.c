#include "global.h"

extern s32 get_random_range(s32);
extern void scene_set_music(struct SongHeader *);
extern u8 D_083AE428[];

void func_0801911C(void) {
    register u8 *base asm("r4") = D_083AE428;
    u32 offset = (u32)get_random_range(2);
    offset = (offset << 0x10) >> 0x0E;
    offset += (u32)base;
    scene_set_music(*(struct SongHeader **)offset);
}
