#include "global.h"
#include "types.h"

struct Func08035FECState {
    u8 padding[0x8E];
    u16 clear;
    u16 first;
    u16 second;
};

void func_08035FEC(struct Func08035FECState *state, u16 second, u16 first)
{
    state->first = first;
    state->second = second;
    state->clear = 0;
}
