#include "global.h"

typedef struct {
    u8 pad0[0x30];
    u32 unk30;
    u32 unk34;
} Func08064D10State;

extern void func_08064D2C(void *, s32);

void func_08064D10(void *arg0) {
    Func08064D10State *state = (Func08064D10State *)arg0;
    u32 value = state->unk34 + 1;

    state->unk34 = value;
    if (value > state->unk30) {
        func_08064D2C(state, 0);
    }
}
