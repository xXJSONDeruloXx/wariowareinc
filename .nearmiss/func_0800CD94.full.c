#include "global.h"

extern void func_0800CD4C(s16, u32, u32);

void func_0800CD94(u32 arg0, u32 arg1) {
    s16 value = *(s16 *)((u8 *)&gBeatscriptScene + 0x1E);

    func_0800CD4C(value, arg0, arg1);
}
