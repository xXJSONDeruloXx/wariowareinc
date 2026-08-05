#include "scenes.h"

extern void func_0800EB50(void);
extern void func_0800C704(u32, const void *);
extern u8 D_083A98D0[];

void func_080102A4(void) {
    func_0800EB50();
    func_0800C704(*(u32 *)((u8 *)gCurrentSceneData + 8), D_083A98D0);
}
