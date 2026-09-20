#include "global.h"
#include "scenes.h"

extern void func_080548B0(void);
extern void func_08054980(void);
extern void func_080549F0(void);

struct ScenePhaseState {
    u8 padding[0x173];
    u8 phase;
};

void func_08054B98(void)
{
    struct ScenePhaseState *state;

    state = (struct ScenePhaseState *)gCurrentSceneData;
    if (state->phase == 1) {
        func_080548B0();
        func_08054980();
        func_080549F0();
    }
}
