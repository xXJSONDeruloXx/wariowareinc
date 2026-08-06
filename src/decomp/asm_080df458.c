#include "global.h"

extern void func_080DF224(void *, void *, u16);

void func_080DF458(void) {
    u8 *base = (u8 *)gCurrentSceneVariable;
    func_080DF224(base + 8, (void *)0x083FB7C0, *(u16 *)(base + 4));
}
