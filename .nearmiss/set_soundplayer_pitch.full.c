#include "global.h"

extern void func_080F2F68(u32 *, s32, s16);

void set_soundplayer_pitch(u32 arg0, u32 arg1) {
    u32 value;

    arg1 <<= 16;
    value = arg1 >> 16;
    if (arg0 != 0) {
        value <<= 16;
        value = (u32)((s32)value >> 16);
        func_080F2F68((u32 *)arg0, 0xFFFF, (s16)value);
    }
}
