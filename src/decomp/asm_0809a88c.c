#include "global.h"
#include "scenes.h"

extern void func_0809A7A8(void);
extern void func_0809A480(void);

struct ScenePhaseState {
    u8 padding[0x173];
    u8 phase;
};

void func_0809A88C(void)
{
    struct ScenePhaseState *state;

    state = (struct ScenePhaseState *)gCurrentSceneData;
    if (state->phase == 1) {
        func_0809A7A8();
    }
    func_0809A480();
}
