#include "scenes.h"

extern void func_0800EB50(void);
extern void func_0800C720(u32, const void *);
extern u8 D_083A98B8[];

void func_0801004C(void) {
    func_0800EB50();
    func_0800C720(*(u32 *)((u8 *)gCurrentSceneData + 8), D_083A98B8);
}
