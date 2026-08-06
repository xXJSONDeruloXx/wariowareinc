#include "global.h"
#include "types.h"

extern void func_08005090(s32, void *, u16, s32, s32);

void func_08005DE0(void *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5) {
    u16 width;
    u32 index;
    u32 base;
    void *destination;

    width = *(u16 *)((u8 *)arg0 + 4);
    index = arg2 + width * arg3;
    index <<= 1;
    base = *(u32 *)arg0;
    destination = (void *)(base + index);
    func_08005090(arg1, destination, width, arg4, arg5);
}
