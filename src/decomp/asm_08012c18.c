#if __INCLUDE_LEVEL__ > 0
#include "global.h"
#include "scenes.h"
#include "types.h"
#include "src/scenes/main_menu.h"

extern u32 save_is_stage_unlocked(u32);
extern u32 get_current_language(void);
extern u32 func_0800068C(u32);
extern u32 D_083AA518;
extern u32 D_083AA4E8;
extern u32 D_083AA500;

u32 func_08012C18(u32 arg0) {
    register u32 r0 asm("r0");
    register u32 r1 asm("r1");
    register u32 r4 asm("r4") = arg0;
    register u32 r5 asm("r5");

    r0 = save_is_stage_unlocked(r4);
    if (r0 != 0) goto unlocked;
    r4 = (u32)&D_083AA518;
    r0 = get_current_language();
    r0 <<= 2;
    r0 += r4;
    goto done;
unlocked:
    r5 = (u32)&D_083AA4E8;
    if (r4 > 0xA) goto skip_bonus;
    r0 = r4;
    r0 = func_0800068C(r0);
    if (r0 == 0) goto skip_bonus;
    r5 = (u32)&D_083AA500;
skip_bonus:
    r0 = get_current_language();
    r0 <<= 2;
    r0 += r5;
    r1 = *(u32 *)r0;
    r0 = r4 << 2;
    r0 += r1;
done:
    r0 = *(u32 *)r0;
    return r0;
}
#endif
