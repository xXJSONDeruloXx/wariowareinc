#if __INCLUDE_LEVEL__ > 0
#include "global.h"
#include "types.h"

extern void *gCurrentSceneData;
extern void func_0800C974(void);
extern u32 get_current_mem_id(void);
extern void schedule_function_call(u16, void *, u32, u32);

typedef void (*Func0800A0C4Schedule)(u32, void *, u32, u32);

void func_0800A0C4(s32 arg0) {
    register u32 r0 asm("r0") = (u32)arg0;
    register u32 r1 asm("r1");
    register u32 r2 asm("r2");
    register u32 r3 asm("r3");
    register u32 r4 asm("r4");
    register u32 r5 asm("r5");

    r3 = r0;
    r5 = (u32)&gCurrentSceneData;
    r1 = *(u32 *)r5;
    r0 = 0xBD;
    r0 <<= 1;
    r2 = r1 + r0;
    r0 = *(u16 *)r2;
    if (r0 != 0) goto done;

    if (r3 == 2) goto special;

    r2 = 0xBC;
    r2 <<= 1;
    r0 = r1 + r2;
    *(u16 *)r0 = r3;
    goto done;

special:
    r0 = 1;
    *(u16 *)r2 = r0;
    r3 = 0xBC;
    r3 <<= 1;
    r0 = r1 + r3;
    r0 = *(u16 *)r0;
    r4 = 2;
    if (r0 != 0) goto have_r4;
    r4 = 1;

have_r4:
    r0 = get_current_mem_id();
    r0 <<= 16;
    r0 >>= 16;
    asm volatile("" : "+r"(r0));
    r1 = (u32)func_0800C974;
    r2 = *(u32 *)r5;
    r3 = 0x27E;
    r2 = r2 + r3;
    r3 = *(u16 *)r2;
    r2 = r4;
    ((Func0800A0C4Schedule)schedule_function_call)(r0, (void *)r1, r2, r3);

done:
    return;
}
#endif
