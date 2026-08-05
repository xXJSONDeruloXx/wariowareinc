#include "global.h"
#include "src/code_08000f10.h"

extern u8 D_03004054[];

void func_08006F04(u32 arg0, u32 arg1, u32 arg2) {
    dma3_set((void *)arg0, D_03004054 + (arg1 << 5), arg2 << 5, 0x20, 0x100);
}
