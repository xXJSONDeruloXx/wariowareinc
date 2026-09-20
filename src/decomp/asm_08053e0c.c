#include "global.h"
#include "scenes.h"

extern void func_0805393C(void);
extern void func_08053AA4(void);
extern void func_08053D38(void);
extern void func_08053C00(void);

struct ScenePhaseState {
    u8 padding[0x173];
    u8 phase;
};

void func_08053E0C(void)
{
    struct ScenePhaseState *state;

    state = (struct ScenePhaseState *)gCurrentSceneData;
    if (state->phase == 1) {
        func_0805393C();
        func_08053AA4();
        func_08053D38();
        func_08053C00();
    }
}
