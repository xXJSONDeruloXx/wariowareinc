#include "src/scenes/main_menu.h"

extern u16 get_current_mem_id(void);
extern s32 func_0800656C(u16, u32, u32, u32, u32, void *, u32, u32, u32);
extern void func_08006790(void *, u32, u32, u32, u32);
extern u8 D_083AAF50[];
extern void func_0801308C(void);
extern void func_08013114(void);

void func_0801312C(void) {
    s32 task;

    task = func_0800656C((u16)get_current_mem_id(),
                         *(u32 *)gCurrentSceneData,
                         *(u32 *)((u8 *)gCurrentSceneData + 0xD0),
                         0xB, 1, D_083AAF50, 0, 0, 0);
    *(u32 *)((u8 *)gCurrentSceneData + 0xE4) = task;
    func_08006790((void *)task, (u32)func_0801308C + 1, 0,
                  (u32)func_08013114 + 1, 0);
}
