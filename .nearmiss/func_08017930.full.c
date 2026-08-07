#include "global.h"
#include "src/scenes/title.h"

struct SceneVariableRoot {
    u32 value;
};

extern void func_080247F8(s32, const void *, u32, u32, u32, u32);
extern void func_080249A4(s32);
extern u16 get_current_mem_id(void);
extern void func_0800A200(u32);
extern void func_08009EE0_stub(u32);
extern struct BeatscriptLocalData *gCurrentSceneVariable;
extern u8 D_083AE20C[];
extern u8 D_083AE218[];

void func_08017930(void) {
    s32 temp_r0;

    start_load_gfx_table_task(get_current_mem_id(), (const struct GraphicsTable *)D_083AE20C, 0x1000);
    temp_r0 = func_080042F4(get_current_mem_id(), D_083ADADC, 0x300, 4, 0x100, 0xA);
    ((struct SceneVariableRoot *)gCurrentSceneVariable)->value = temp_r0;
    func_080247F8(temp_r0, D_083AE218, 0x800, 0xD, 0xE, 0xF);
    func_080249A4(0x3C);
    func_0800A200(0);
    func_08009EE0_stub(0);
}
