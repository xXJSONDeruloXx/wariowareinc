#include "global.h"
#include "types.h"

extern u8 D_030006A0;
extern void task_stop(void *, s32);

void func_080059E4(u32 arg0) {
    register u32 raw asm("r0") = arg0;
    u32 taskId;
    register u32 count asm("r5");
    u8 *task;
    register u32 compareId asm("r6");

    raw <<= 16;
    count = 0;
    task = &D_030006A0;
    taskId = raw >> 16;
    compareId = taskId;
    do {
        if ((task[0] & 1) != 0) {
            *(u16 *)(task + 2) = taskId;
            if (compareId != 0) {
                task_stop(task, 0);
            }
        }
        count += 1;
        task += 0x1C;
    } while (count <= 0x2F);
}
