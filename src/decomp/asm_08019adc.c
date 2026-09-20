#include "global.h"
#include "types.h"

extern void func_0800CF3C(const void *);
extern void func_0800CF5C(const void *);

struct Func08019ADCState {
    u8 padding[0xD0];
    void *source;
};

void func_08019ADC(void)
{
    struct Func08019ADCState *state;

    state = (struct Func08019ADCState *)gCurrentSceneVariable;
    func_0800CF3C(state->source);
    state = (struct Func08019ADCState *)gCurrentSceneVariable;
    func_0800CF5C(state->source);
}
