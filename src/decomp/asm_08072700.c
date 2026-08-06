#include "global.h"
#include "scenes.h"

extern void func_0807249C(void);

void func_08072700(s32 arg0) {
    u8 *base = (u8 *)gCurrentSceneVariable + (0xE7 << 3);

    *(u32 *)base = *(u32 *)base + arg0;
    func_0807249C();
}
