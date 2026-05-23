#if __INCLUDE_LEVEL__ > 0
#include "global.h"

extern u32 save_is_stage_unlocked(u32);
extern u32 get_current_language(void);
extern u32 D_083AAF20[];
extern u32 D_083AAF38[];

u32 func_080135E8(u32 stage) {
    register u32 r0 asm("r0");
    register u32 r1 asm("r1");
    register u32 r4 asm("r4") = stage;

    r0 = save_is_stage_unlocked(r4);
    if (r0 == 0) goto not_unlocked;
    r0 = get_current_language();
    r1 = (u32)D_083AAF20;
    r0 <<= 2;
    r0 += r1;
    r1 = *(u32 *)r0;
    r0 = r4 << 2;
    r0 += r1;
    goto done;
    not_unlocked:
    r4 = (u32)D_083AAF38;
    r0 = get_current_language();
    r0 <<= 2;
    r0 += r4;
    done:
    r0 = *(u32 *)r0;
    return r0;
}
#endif
