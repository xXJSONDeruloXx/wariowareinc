#include "global.h"
#include "scenes.h"

extern void func_080694E8(void);
extern void func_080695EC(void);
extern void func_08069670(void);
extern void func_080693A4(void);
extern void func_08069794(void);

struct ScenePhaseState {
    u8 padding[0x173];
    u8 phase;
};

void func_0806980C(void)
{
    struct ScenePhaseState *state;

    state = (struct ScenePhaseState *)gCurrentSceneData;
    if (state->phase == 1) {
        func_080694E8();
        func_080695EC();
        func_08069670();
        func_080693A4();
    }
    func_08069794();
}
