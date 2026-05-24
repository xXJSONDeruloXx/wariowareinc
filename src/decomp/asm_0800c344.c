#if __INCLUDE_LEVEL__ > 0
#include "global.h"

extern u32 get_current_mem_id(void);
extern void *start_new_task(u16, void *, void *, void *, u32);
extern u8 D_083A4AC0;

typedef struct {
    u16 unk0;
    u8 unk2;
    u8 pad3;
    u16 unk4;
    u16 unk6;
    u16 unk8;
    u16 unkA;
    u16 unkC;
    u16 unkE;
    u16 unk10;
} Func0800C344TaskArgs;

void *func_0800C344(s16 arg0, u8 arg1, s16 arg2, s16 arg3, s16 arg4, s16 arg5, s16 arg6, s16 arg7, s16 arg8) {
    Func0800C344TaskArgs stackArgs;
    stackArgs.unk0 = arg0;
    stackArgs.unk2 = arg1;
    stackArgs.unk4 = arg2;
    stackArgs.unk6 = arg3;
    stackArgs.unk8 = arg4;
    stackArgs.unkA = arg5;
    stackArgs.unkC = arg6;
    stackArgs.unkE = arg7;
    stackArgs.unk10 = arg8;
    return start_new_task((u16)get_current_mem_id(), &D_083A4AC0, &stackArgs, 0, 0);
}
#endif
