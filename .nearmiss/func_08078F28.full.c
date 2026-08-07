#include "global.h"

typedef struct {
    u8 pad0[6];
    s16 field6;
    u8 pad8[2];
    u16 fieldA;
    s32 fieldC;
} Func08078F28State;

void func_08078F28(Func08078F28State *state) {
    s32 quotient = 0x80000 / state->fieldC;
    state->field6 = (s16)(state->fieldA + 8 - quotient);
}
