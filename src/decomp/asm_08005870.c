#include "global.h"
#include "types.h"

extern u8 D_030006A0;
extern void task_stop(void *, s32);

typedef struct {
    u8 flags;
    u8 padding1[7];
    s32 taskId;
    u8 paddingC[0x10];
} Func08005870Task;

void func_08005870(s32 arg0) {
    Func08005870Task *task;
    u32 index;
    s32 compareId;
    u32 value;

    compareId = arg0;
    if (compareId < 0) goto done;
    index = 0;
    task = (Func08005870Task *)&D_030006A0;
    goto scan;
next:
    index += 1;
    task++;
    if (index > 0x2F) goto done;
scan:
    if (task->taskId != compareId) goto next;
    if (index > 0x2F) goto done;
    index = task->flags;
    value = 1;
    value &= index;
    if (value == 0) goto done;
    task_stop(task, 1);
done:;
}
