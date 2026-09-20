#include "global.h"
#include "scenes.h"

extern void func_080A4274(void);
extern void func_080A41B4(void);

struct ScenePhaseState {
    u8 padding[0x173];
    u8 phase;
};

void func_080A42E8(void)
{
    struct ScenePhaseState *state;

    state = (struct ScenePhaseState *)gCurrentSceneData;
    if (state->phase == 1) {
        func_080A4274();
        func_080A41B4();
    }
}
