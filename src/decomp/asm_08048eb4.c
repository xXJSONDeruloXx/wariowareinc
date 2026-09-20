#include "global.h"
#include "scenes.h"

extern void func_08048CC8(void);
extern void func_08048DE8(void);

struct ScenePhaseState {
    u8 padding[0x173];
    u8 phase;
};

void func_08048EB4(void)
{
    struct ScenePhaseState *state;

    state = (struct ScenePhaseState *)gCurrentSceneData;
    if (state->phase == 1) {
        func_08048CC8();
        func_08048DE8();
    }
}
