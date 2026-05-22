#if __INCLUDE_LEVEL__ > 0
#include "global.h"

extern u8 D_083ADADC[];
extern u32 func_0800430C(u32, void *, u32, u32);
extern void func_0800D23C(void);
extern u32 get_current_mem_id(void);

u32 func_0800A3FC(u32 arg0, u32 arg1) {
    u32 result;
    u32 tex;
    u32 pal;

    tex = (u16)arg0;
    pal = (u8)arg1;
    result = func_0800430C((u16)get_current_mem_id(), D_083ADADC, tex, pal);
    func_0800D23C();
    return result;
}
#endif
