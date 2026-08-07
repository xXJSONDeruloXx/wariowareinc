#include "global.h"
#include "src/scenes/title.h"

struct SceneVariableRoot {
    u32 value;
};

extern void func_0800A3A4(s32);
extern void func_08006A04(void);
extern void func_08006B90(u32);
extern void func_0800A200(u32);
extern void func_08009EE0_stub(u32);
extern struct BeatscriptLocalData *gCurrentSceneVariable;
extern u8 D_083ADADC[];

void func_080178C4(void) {
    func_0800A3A4(1);
    func_08006A04();
    func_08006B90(0);
    ((struct SceneVariableRoot *)gCurrentSceneVariable)->value =
        func_080042F4(1, D_083ADADC, 0x180, 4, 0x100, 0xA);
    func_0800A200(0);
    func_08009EE0_stub(1);
}
