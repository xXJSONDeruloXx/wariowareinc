#include "global.h"

typedef struct {
    u8 pad0[0x3C];
    s32 field3C;
    u8 pad40[0x11];
    u8 field51;
} Func080B39F0State;

extern void func_080B3A0C(Func080B39F0State *state);

void func_080B39F0(Func080B39F0State *state) {
    if (state->field51 > 1 && state->field3C <= 0) {
        func_080B3A0C(state);
    }
}
