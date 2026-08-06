#include "global.h"

extern void func_080DF224(void *, void *, u16);

void func_0803E9A0(void) {
    u8 *base = (u8 *)gCurrentSceneVariable;

    func_080DF224(base + (0x86 << 1), (void *)0x083FB478,
                  *(u16 *)(base + 0x60));
}
