#include "global.h"
extern void scene_set_current_thread(u32);
extern void sprite_set_enable_updates(s32, s16, s32);
void func_0804A73C(s32 a, u16 b) { scene_set_current_thread(1); sprite_set_enable_updates(a, (s16)b, 1); }
