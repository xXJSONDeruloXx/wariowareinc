#include "global.h"

extern s32 play_sound(s32);
extern void func_08002038(s32, u32);

void func_080DF224(s32 *arg0, s32 arg1, u32 arg2) {
    s32 value = play_sound(arg1);

    *arg0 = value;
    func_08002038(value, (u16)arg2);
}
