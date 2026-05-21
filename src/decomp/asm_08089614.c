#include "global.h"

extern s32 __divsi3(s32, s32);

s32 func_08089614(s32 a0, s32 a1, s32 a2) {
    return __divsi3(a2 << 8, a1);
}