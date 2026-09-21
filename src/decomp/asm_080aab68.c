#include "global.h"
extern s32 play_sound(void);
extern void func_0800C934(s32, s16);
s32 func_080AAB68(s32 unused, u16 soundParam) { s32 handle; handle = play_sound(); func_0800C934(handle, (s16)soundParam); return handle; }
