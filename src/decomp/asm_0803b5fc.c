#include "global.h"
#include "scenes.h"

extern void func_0803B288(void);
extern void func_0803AC54(void);
extern void func_0803B4D0(void);

struct ScenePhaseState {
    u8 padding[0x173];
    u8 phase;
};

void func_0803B5FC(void)
{
    struct ScenePhaseState *state;

    state = (struct ScenePhaseState *)gCurrentSceneData;
    if (state->phase == 1) {
        func_0803B288();
        func_0803AC54();
        func_0803B4D0();
    }
}
