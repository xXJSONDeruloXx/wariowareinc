#include "global.h"
#include "scenes.h"

extern void func_08082C2C(void);
extern void func_08082BD8(void);
extern void func_080828C4(void);

struct ScenePhaseState {
    u8 padding[0x173];
    u8 phase;
};

void func_08082EEC(void)
{
    struct ScenePhaseState *state;

    state = (struct ScenePhaseState *)gCurrentSceneData;
    if (state->phase == 1) {
        func_08082C2C();
        func_08082BD8();
    }
    func_080828C4();
}
