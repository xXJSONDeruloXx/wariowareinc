#include "global.h"
#include "scenes.h"

extern void func_080B44B4(void);
extern void func_080B4544(void);

struct ScenePhaseState {
    u8 padding[0x173];
    u8 phase;
};

void func_080B4684(void)
{
    struct ScenePhaseState *state;

    state = (struct ScenePhaseState *)gCurrentSceneData;
    if (state->phase == 1) {
        func_080B44B4();
    }
    func_080B4544();
}
