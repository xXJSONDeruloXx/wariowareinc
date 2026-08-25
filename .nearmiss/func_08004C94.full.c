#include "global.h"

extern void *start_new_task(u16, void *, void *, void *, u32);
extern u8 D_083A49EC;

void *func_08004C94(u32 arg0, u32 arg1, u32 arg2, u32 arg3) {
    u32 stackArgs[4];
    u16 id;

    id = (u16)arg0;
    stackArgs[0] = arg1;
    stackArgs[1] = arg2;
    stackArgs[2] = arg3;
    return start_new_task(id, &D_083A49EC, &stackArgs[0], 0, 0);
}
