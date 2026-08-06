#include "global.h"

extern u16 get_current_mem_id(void);
extern s32 schedule_function_call(u16, void *, s32, u32);
extern void func_08016EC8(void);

void func_08016EF8(void) {
    schedule_function_call(get_current_mem_id(), (void *)((u8 *)&func_08016EC8 + 1), 0, 2);
}
