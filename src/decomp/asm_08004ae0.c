#include "global.h"

extern s32 func_08004B00(u32, u32, s32, s32, s32, s32);

s32 func_08004AE0(u32 arg0, u32 arg1, s16 arg2, s16 arg3) {
    func_08004B00(arg0, arg1, (s32)arg2, (s32)arg3, 0, 0);
}
