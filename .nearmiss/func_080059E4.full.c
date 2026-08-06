#include "global.h"
#include "types.h"

extern u8 D_030006A0;
extern void task_stop(void *, s32);

void func_080059E4(u16 arg0) {
    u32 count;
    u8 *task;
    u16 taskId;

    count = 0;
    task = &D_030006A0;
    taskId = arg0;
    do {
        if ((task[0] & 1) != 0) {
            *(u16 *)(task + 2) = taskId;
            if (taskId != 0) {
                task_stop(task, 0);
            }
        }
        count += 1;
        task += 0x1C;
    } while (count <= 0x2F);
}
