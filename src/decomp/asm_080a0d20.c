#include "global.h"
#include "scenes.h"

extern void func_080A052C(void);
extern void func_080A0A0C(void);
extern void func_080A0B1C(void);

struct ScenePhaseState {
    u8 padding[0x173];
    u8 phase;
};

void func_080A0D20(void)
{
    struct ScenePhaseState *state;

    state = (struct ScenePhaseState *)gCurrentSceneData;
    if (state->phase == 1) {
        func_080A052C();
        func_080A0A0C();
        func_080A0B1C();
    }
}
