#include "global.h"
#include "src/scenes/gameplay.h"

extern u32 func_0800A3FC(u32, u32);

void func_08025514(void) {
    *(u32 *)((u8 *)D_0300652C + 4) = func_0800A3FC(0x180, 4);
}
