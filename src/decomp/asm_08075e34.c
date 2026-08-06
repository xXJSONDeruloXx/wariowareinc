#include "global.h"

extern void func_080DF224(void *, void *, u16);

void func_08075E34(void) {
    u32 result;

    func_080DF224(&result, (void *)0x083FBE78,
                  *(u16 *)((u8 *)gCurrentSceneVariable + 0x28));
}
