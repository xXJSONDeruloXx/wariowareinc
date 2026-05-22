#if __INCLUDE_LEVEL__ > 0
#include "global.h"

extern void *start_new_task(u16, void *, void *, void *, u32);
extern void *D_083A4494;

void *start_load_gfx_table_task(u16 arg0, void *arg1, void *arg2) {
    void *stack_args[2];
    stack_args[0] = arg1;
    stack_args[1] = arg2;
    return start_new_task(arg0, &D_083A4494, &stack_args[0], NULL, 0);
}
#endif
