#include "global.h"
#include "types.h"

extern void func_0808A7E4(void);

struct Func0808A898State {
    u8 padding[0x40];
    u16 threshold;
    u16 value;
};

void func_0808A898(void)
{
    struct Func0808A898State *state;

    state = (struct Func0808A898State *)gCurrentSceneVariable;
    if (state->value < state->threshold) {
        func_0808A7E4();
    }
}
