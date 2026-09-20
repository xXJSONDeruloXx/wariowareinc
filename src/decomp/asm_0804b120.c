#include "global.h"
#include "scenes.h"

extern void func_0804AAB4(void);
extern void func_0804ACEC(void);
extern void func_0804ADE8(void);
extern void func_0804AE74(void);

struct ScenePhaseState {
    u8 padding[0x173];
    u8 phase;
};

void func_0804B120(void)
{
    struct ScenePhaseState *state;

    state = (struct ScenePhaseState *)gCurrentSceneData;
    if (state->phase == 1) {
        func_0804AAB4();
        func_0804ACEC();
        func_0804ADE8();
        func_0804AE74();
    }
}
