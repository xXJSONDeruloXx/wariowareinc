#include "global.h"
#include "scenes.h"

extern void func_080453E0(void);
extern void func_0804520C(void);
extern void func_08044FBC(void);

struct ScenePhaseState {
    u8 padding[0x173];
    u8 phase;
};

void func_080454D8(void)
{
    struct ScenePhaseState *state;

    state = (struct ScenePhaseState *)gCurrentSceneData;
    if (state->phase == 1) {
        func_080453E0();
        func_0804520C();
    }
    func_08044FBC();
}
