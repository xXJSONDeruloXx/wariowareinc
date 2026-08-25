#include "global.h"

typedef struct Func08006700State Func08006700State;
typedef void (*Func08006700Callback)(Func08006700State *, void *);

struct Func08006700State {
    u8 pad0[0x1C];
    u16 field1C;
    u8 pad1E[0xA];
    Func08006700Callback callback;
    void *callbackArg;
};

extern void func_08006648(Func08006700State *state);

void func_08006700(Func08006700State *state) {
    if ((((u32)state->field1C << 20) >> 20) == 1) {
        func_08006648(state);
        if (state->callback != 0) {
            state->callback(state, state->callbackArg);
        }
        {
            u32 value;
            u32 mask;

            value = state->field1C;
            mask = 0xFFFFF000;
            mask &= value;
            state->field1C = mask;
        }
    }
}
