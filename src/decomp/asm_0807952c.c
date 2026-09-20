#include "global.h"
#include "scenes.h"

extern void func_0807945C(void);
extern void func_08079358(void);
extern void func_08079128(void);

struct ScenePhaseState {
    u8 padding[0x173];
    u8 phase;
};

void func_0807952C(void)
{
    struct ScenePhaseState *state;

    state = (struct ScenePhaseState *)gCurrentSceneData;
    if (state->phase == 1) {
        func_0807945C();
        func_08079358();
        func_08079128();
    }
}
