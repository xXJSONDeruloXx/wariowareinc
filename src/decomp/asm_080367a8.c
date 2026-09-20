#include "global.h"

struct Func080367A8State {
    u8 padding[0x94];
    s8 first;
    s8 second;
};

void func_08001B28(s32);

void func_080367A8(struct Func080367A8State *state) {
    func_08001B28(state->first);
    func_08001B28(state->second);
}
