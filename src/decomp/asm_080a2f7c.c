#include "global.h"
#include "scenes.h"

extern void func_080A2A18(void);
extern void func_080A2EB4(void);

struct ScenePhaseState {
    u8 padding[0x173];
    u8 phase;
};

void func_080A2F7C(void)
{
    struct ScenePhaseState *state;

    state = (struct ScenePhaseState *)gCurrentSceneData;
    if (state->phase == 1) {
        func_080A2A18();
        func_080A2EB4();
    }
}
