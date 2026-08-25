#include "global.h"

typedef struct {
    u8 pad0[4];
    s32 field4;
} Func080B3690State;

void func_080B3690(Func080B3690State *state) {
    if (state->field4 > 0xF000) {
        state->field4 = 0;
    }
    if (state->field4 < 0) {
        state->field4 = 0xF000;
    }
}
