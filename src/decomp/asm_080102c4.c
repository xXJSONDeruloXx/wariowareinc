#include "global.h"
#include "scenes.h"

extern void func_0800C720(u32 arg0, const void *arg1);
extern u8 D_083A98D0[];

void func_080102C4(void) {
    func_0800C720(*(u32 *)((u8 *)gCurrentSceneData + 8), D_083A98D0);
}
