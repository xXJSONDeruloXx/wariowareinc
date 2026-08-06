#include "scenes.h"

extern void func_080DF224(void *, void *, u16);

void func_080B2704(void) {
    u32 result;

    func_080DF224(&result, (void *)0x083FBA90,
                  *(u16 *)((u8 *)gCurrentSceneData + 0x16));
}
