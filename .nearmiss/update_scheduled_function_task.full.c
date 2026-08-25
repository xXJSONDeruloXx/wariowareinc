#include "global.h"

struct Func08007DCCTask {
    void (*function)(void *argument);
    void *argument;
    u32 delay;
};

u32 update_scheduled_function_task(struct Func08007DCCTask *task) {
    if (task->delay != 0) {
        task->delay--;
        return 0;
    }
    if (task->function != 0) {
        task->function(task->argument);
    }
    return 1;
}
