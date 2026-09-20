#include "global.h"
#include "scenes.h"

extern void func_08055BB0(void);
extern void func_08055C48(void);

struct ScenePhaseState {
    u8 padding[0x173];
    u8 phase;
};

void func_08055F74(void)
{
    struct ScenePhaseState *state;

    state = (struct ScenePhaseState *)gCurrentSceneData;
    if (state->phase == 1) {
        func_08055BB0();
        func_08055C48();
    }
}
