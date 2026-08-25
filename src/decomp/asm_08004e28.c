#include "global.h"

typedef struct {
    void *field0;
} Func08004E28State;

extern void *func_08004CE0(void *arg0, void *arg1, u8 arg2);
extern void mem_heap_dealloc(void *ptr);

void func_08004E28(void *arg0, Func08004E28State *state, s32 arg2) {
    u8 value = (u8)arg2;
    void *result;

    result = func_08004CE0(arg0, state->field0, value);
    mem_heap_dealloc(state->field0);
    state->field0 = result;
}
