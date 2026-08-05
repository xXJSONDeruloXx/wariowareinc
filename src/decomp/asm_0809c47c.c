#include "global.h"

typedef void (*Func0809C47CFinal)(void);

void run_func_after_task(u32, Func0809C47CFinal, u32);
void func_0809C41C(void);

u32 func_0809C47C(u32 task) {
    run_func_after_task(task, (Func0809C47CFinal)(func_0809C41C + 1), 0);
}
