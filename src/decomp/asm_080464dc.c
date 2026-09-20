#include "global.h"
#include "scenes.h"

extern void func_080462A8(void);
extern void func_08046474(void);
extern void func_080460AC(void);

struct ScenePhaseState {
    u8 padding[0x173];
    u8 phase;
};

void func_080464DC(void)
{
    struct ScenePhaseState *state;

    state = (struct ScenePhaseState *)gCurrentSceneData;
    if (state->phase == 1) {
        func_080462A8();
        func_08046474();
        func_080460AC();
    }
}
