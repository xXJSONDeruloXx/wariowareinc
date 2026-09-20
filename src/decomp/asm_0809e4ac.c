#include "global.h"
#include "scenes.h"

extern void func_0809E1A8(void);
extern void func_0809E418(void);

struct ScenePhaseState {
    u8 padding[0x173];
    u8 phase;
};

void func_0809E4AC(void)
{
    struct ScenePhaseState *state;

    func_0809E1A8();
    state = (struct ScenePhaseState *)gCurrentSceneData;
    if (state->phase <= 1) {
        func_0809E418();
    }
}
