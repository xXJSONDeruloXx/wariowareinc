#include "global.h"

struct Func0802FB98State {
    u8 padding[0x80];
    u32 count;
};

extern void func_0802F9CC(struct Func0802FB98State *state);
extern void func_0802F6C0(struct Func0802FB98State *state);

void func_0802FB98(struct Func0802FB98State *state) {
    state->count++;
    func_0802F9CC(state);
    func_0802F6C0(state);
}
