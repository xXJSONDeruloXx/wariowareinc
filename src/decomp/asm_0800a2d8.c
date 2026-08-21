#if __INCLUDE_LEVEL__ > 0
#include "global.h"

extern u32 get_current_mem_id(void);
extern void *start_new_task(u32, void *, void *, void *, u32);
extern u8 D_083A4AF0[];

typedef struct {
    u32 lo : 2;
    u32 mid : 15;
    u32 hi : 15;
    u32 w1;
    u32 w2;
    u32 w3;
} Func0800A2D8TaskArgs;

void *func_0800A2D8(u32 arg0, u32 arg1, u32 arg2, u32 arg3, u32 arg4) {
    Func0800A2D8TaskArgs taskArgs;

    taskArgs.lo = arg0;
    taskArgs.mid = arg1;
    taskArgs.w1 = arg2;
    taskArgs.w2 = arg3;
    taskArgs.w3 = arg4;
    return start_new_task((u16)get_current_mem_id(), D_083A4AF0, &taskArgs, NULL, 0);
}
#endif
