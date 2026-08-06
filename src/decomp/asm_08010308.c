#include "scenes.h"

extern void func_0800C704(u32, const void *);
extern void func_0800FFA8(u32);
extern u8 D_083A98D8[];

void func_08010308(void) {
    func_0800C704(*(u32 *)((u8 *)gCurrentSceneData + 8), D_083A98D8);
    func_0800FFA8(0);
}
