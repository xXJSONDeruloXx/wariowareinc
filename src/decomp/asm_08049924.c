#include "global.h"
#include "scenes.h"

extern void func_08049948(void);
extern void func_08049B44(void);

struct ScenePhaseState {
    u8 padding[0x173];
    u8 phase;
};

void func_08049924(void)
{
    struct ScenePhaseState *state;

    state = (struct ScenePhaseState *)gCurrentSceneData;
    if (state->phase <= 1) {
        func_08049948();
        func_08049B44();
    }
}
