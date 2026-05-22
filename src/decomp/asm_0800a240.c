#if __INCLUDE_LEVEL__ > 0
#include "global.h"

extern u32 get_current_mem_id(void);
extern void *start_new_task(u32 memID, void *unk1, void *unk2, void *unk3, u32 stackArg);

void *func_0800A240(void *arg0, void *arg1, void *arg2, u32 arg3) {
    return start_new_task((u16)get_current_mem_id(), arg0, arg1, arg2, arg3);
}
#endif
