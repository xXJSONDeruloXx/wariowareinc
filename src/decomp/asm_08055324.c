#include "global.h"
#include "scenes.h"

extern void func_080554D4(void);
extern void func_08055730(void);

struct ScenePhaseState {
    u8 padding[0x173];
    u8 phase;
};

void func_08055324(void)
{
    struct ScenePhaseState *state;

    state = (struct ScenePhaseState *)gCurrentSceneData;
    if (state->phase <= 1) {
        func_080554D4();
    }
    func_08055730();
}
