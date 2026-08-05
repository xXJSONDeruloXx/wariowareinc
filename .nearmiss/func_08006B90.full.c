#include "global.h"
#include "src/code_08000f10.h"

extern u8 D_03004054[];

void func_08006B90(u16 arg0) {
    u32 value = (u16)arg0;

    value |= value << 16;
    dma3_fill(value, D_03004054, 0x400, 0x20, 0x100);
}
