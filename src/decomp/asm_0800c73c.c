#include "global.h"

typedef struct {
    u16 field0;
    u16 field2;
    u16 field4;
    u16 field6;
    u16 field8;
} Func0800C73CState;

extern void *func_0800A228(u32 size);
extern s32 func_0800A218(void);

Func0800C73CState *func_0800C73C(void) {
    Func0800C73CState *state = (Func0800C73CState *)func_0800A228(0xC);

    state->field0 = func_0800A218();
    state->field2 = 0x100;
    state->field4 = 0;
    state->field6 = 0;
    state->field8 = 0x100;
    return state;
}
