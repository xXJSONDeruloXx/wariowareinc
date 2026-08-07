#include "global.h"

typedef struct {
    s32 field0;
    u8 pad4[0xC];
    s32 field10;
} Func080CF440State;

extern void func_08001BA4(s32, s32, s32);

void func_080CF440(Func080CF440State *state) {
    s32 target = state->field0;
    s32 value = state->field10;

    if (value < 0) {
        value += 0x3F;
    }
    func_08001BA4(target, (value << 10) >> 16, 0);
}
