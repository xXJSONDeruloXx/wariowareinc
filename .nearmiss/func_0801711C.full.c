#include "global.h"

extern void dma3_set(s32 source, void *destination, u32 bytesToSet, u16 unit, u32 bytesPerInterrupt);
extern void func_08003DF4(s32 source, void *destination);

void func_0801711C(s32 arg0) {
    s32 temp_r3;

    temp_r3 = arg0 & 0x7FFFFFFF;
    if (0x80000000 & arg0) {
        dma3_set(temp_r3, (void *)(VRAMBase + 0xE000), 0x800, 0x20, 0x100);
        return;
    }
    func_08003DF4(temp_r3, (void *)(VRAMBase + 0xE000));
}
