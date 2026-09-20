#include "global.h"

struct Func08027720State {
    u8 padding[0x68];
    s8 first;
    s8 second;
};

void func_08001B28(s32);

void func_08027720(struct Func08027720State *state) {
    func_08001B28(state->first);
    func_08001B28(state->second);
}
