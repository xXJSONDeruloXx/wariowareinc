#include "global.h"

void func_0806F0A0(void) {
    u8 *p = (u8 *)gCurrentSceneVariable;
    s16 delta = *(s16 *)(p + 0xC);
    s32 value = *(s32 *)(p + 4);

    value += delta;
    *(s32 *)(p + 4) = value;
    if (value > 0xD000) {
        *(s32 *)(p + 4) = 0xD000;
    }
}
