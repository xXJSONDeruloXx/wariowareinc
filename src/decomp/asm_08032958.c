#include "global.h"

struct Func08032958State {
    u8 padding[0x5C];
    s8 first;
    s8 second;
};

void func_08001B28(s32);

void func_08032958(struct Func08032958State *state) {
    func_08001B28(state->second);
    func_08001B28(state->first);
}
