#if __INCLUDE_LEVEL__ > 0
#include "src/scenes/main_menu.h"

extern u16 get_current_mem_id(void);
extern s32 func_0800656C(u16, u32, u32, u32, u32, void *, u32, u32, u32);
extern void func_08006790(void *, u32, u32, u32, u32);
extern u8 D_083AB374[];
extern void func_08014878(void);
extern void func_080148BC(void);

void func_080148EC(void) {
    s32 task;

    task = func_0800656C((u16)get_current_mem_id(),
                         *(u32 *)gCurrentSceneData,
                         *(u32 *)((u8 *)gCurrentSceneData + 0xD0),
                         6, 7, D_083AB374, 0, 0, 0);
    *(u32 *)((u8 *)gCurrentSceneData + 0x140) = task;
    func_08006790((void *)task, (u32)func_08014878 + 1, 0,
                  (u32)func_080148BC + 1, 0);
}
#endif
