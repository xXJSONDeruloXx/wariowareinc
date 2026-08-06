#include "global.h"

extern void func_080D3BE8(void);

void func_080D3A60(void) {
    func_080D3BE8();
    *(u16 *)((u8 *)gCurrentSceneVariable + (0xDF << 2)) = 1;
}
