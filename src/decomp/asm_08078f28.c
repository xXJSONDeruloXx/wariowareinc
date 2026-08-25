#include "global.h"

typedef struct {
    u8 pad0[6];
    s16 field6;
    u8 pad8[2];
    u16 fieldA;
    s32 fieldC;
} Func08078F28State;

extern s32 __divsi3(s32, s32);

void func_08078F28(Func08078F28State *state) {
    s32 value = state->fieldA;
    s32 denominator;
    s32 quotient;

    value += 8;
    denominator = state->fieldC;
    quotient = __divsi3(0x80000, denominator);
    state->field6 = value - quotient;
}
