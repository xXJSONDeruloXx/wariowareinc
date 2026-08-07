#include "global.h"

typedef struct {
    u32 field0;
    u32 field4;
    u32 field8;
} ScheduledFunctionTask;

extern void *mem_heap_alloc(u32 size);

void init_scheduled_function_task(const ScheduledFunctionTask *source) {
    ScheduledFunctionTask *task = (ScheduledFunctionTask *)mem_heap_alloc(0xC);

    task->field0 = source->field0;
    task->field4 = source->field4;
    task->field8 = source->field8;
}
