#include "global.h"
#include "scenes.h"

extern void func_0805E530(void);
extern void func_0805E180(void);
extern void func_0805E1F4(void);

struct ScenePhaseState {
    u8 padding[0x173];
    u8 phase;
};

void func_0805E6C4(void)
{
    struct ScenePhaseState *state;

    state = (struct ScenePhaseState *)gCurrentSceneData;
    if (state->phase == 1) {
        func_0805E530();
    }
    func_0805E180();
    func_0805E1F4();
}
