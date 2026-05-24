#if __INCLUDE_LEVEL__ > 0
#include "global.h"

extern u32 get_current_mem_id(void);
extern void *start_new_task(u16, void *, void *, void *, u32);
extern u8 D_083A4AB0;

typedef struct {
    u16 unk0;
    u16 unk2;
    u16 unk4;
    u16 unk6;
    u16 unk8;
    u16 unkA;
} Func0800C298TaskArgs;

void *func_0800C298(s16 arg0, u16 arg1, s16 arg2, s16 arg3, s16 arg4, s16 arg5) {
    Func0800C298TaskArgs stackArgs;
    stackArgs.unk0 = arg0;
    stackArgs.unk2 = arg1;
    stackArgs.unk4 = arg2;
    stackArgs.unk6 = arg3;
    stackArgs.unk8 = arg4;
    stackArgs.unkA = arg5;
    return start_new_task((u16)get_current_mem_id(), &D_083A4AB0, &stackArgs, 0, 0);
}
#endif
