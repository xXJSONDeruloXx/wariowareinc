#include "global.h"

extern u16 get_random_range(u16 value);
extern void scene_set_music_pitch_env(s16 value);

void func_0801CB24(u32 arg0) {
    u16 value = get_random_range((u16)arg0);

    scene_set_music_pitch_env((s16)((arg0 >> 1) - value));
}
