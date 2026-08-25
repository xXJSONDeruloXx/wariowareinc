#include "global.h"

struct ScheduleFunctionCallArgs {
    void *function;
    s32 param;
    u32 delay;
};

extern s32 start_new_task(u16 memID, void *task, void *stackArgs, void *arg3, u32 arg4);
extern u8 D_083A4B38;

s32 schedule_function_call(u32 memID, void *function, s32 param, u32 delay) {
    struct ScheduleFunctionCallArgs stackArgs;
    u16 id;

    id = (u16)memID;
    stackArgs.function = function;
    stackArgs.param = param;
    stackArgs.delay = delay;
    return start_new_task(id, &D_083A4B38, &stackArgs.function, 0, 0);
}
