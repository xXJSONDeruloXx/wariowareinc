#include "global.h"
#include "types.h"

extern u8 D_030006A0;
extern void task_stop(void *, s32);

typedef struct {
    u8 flags;
    u8 padding1[7];
    s32 taskId;
    u8 paddingC[0x10];
} Func080058ACTask;

void func_080058AC(void) {
    Func080058ACTask *task;
    u32 index;
    u32 flags;
    u32 value;
    s32 taskId;

    index = 0;
    task = (Func080058ACTask *)&D_030006A0;
loop:
    value = task->flags;
    flags = 1;
    flags &= value;
    if (flags == 0) goto next;
    taskId = task->taskId;
    if (taskId < 0) goto next;
    task_stop(task, 1);
next:
    index += 1;
    task++;
    if (index <= 0x2F) goto loop;
}
