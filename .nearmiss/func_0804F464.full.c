#include "global.h"

typedef struct {
    u8 pad0[4];
    s32 field4;
    s32 field8;
    s32 fieldC;
    s32 field10;
    s32 field14;
    s32 field18;
    s32 field1C;
    u8 field20;
} Func0804F464State;

void func_0804F464(Func0804F464State *state, s32 arg1, s32 arg2, s32 arg3) {
    state->field4 = arg1;
    state->field8 = arg2;
    state->fieldC = arg3;
    state->field14 = 0;
    state->field10 = 0;
    state->field18 = 0;
    state->field1C = 0;
    state->field20 &= 0xF0;
}
