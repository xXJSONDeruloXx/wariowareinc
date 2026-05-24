#if __INCLUDE_LEVEL__ > 0
#include "global.h"

extern u32 get_current_mem_id(void);
extern void *start_new_task(u16, void *, void *, void *, u32);
extern u8 D_083A4AA0;

typedef struct {
    u16 unk0;
    u16 unk2;
    u16 unk4;
    u16 unk6;
    u16 unk8;
    u16 unkA;
    u16 unkC;
} Func0800C1C0TaskArgs;

void *func_0800C1C0(s16 arg0, u16 arg1, s16 arg2, s16 arg3, s16 arg4, s16 arg5, s16 arg6) {
    Func0800C1C0TaskArgs stackArgs;
    stackArgs.unk0 = arg0;
    stackArgs.unk2 = arg1;
    stackArgs.unk4 = arg2;
    stackArgs.unk6 = arg3;
    stackArgs.unk8 = arg4;
    stackArgs.unkA = arg5;
    stackArgs.unkC = arg6;
    return start_new_task((u16)get_current_mem_id(), &D_083A4AA0, &stackArgs, 0, 0);
}
#endif
