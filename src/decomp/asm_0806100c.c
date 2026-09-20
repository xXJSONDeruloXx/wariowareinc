#include "global.h"
#include "scenes.h"

extern void func_08060F08(void);
extern void func_08060CDC(void);

struct ScenePhaseState {
    u8 padding[0x173];
    u8 phase;
};

void func_0806100C(void)
{
    struct ScenePhaseState *state;

    state = (struct ScenePhaseState *)gCurrentSceneData;
    if (state->phase == 1) {
        func_08060F08();
        func_08060CDC();
    }
}
