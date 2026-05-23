#if __INCLUDE_LEVEL__ > 0
#include "global.h"

extern void func_080024A4(void *, void *, s32);

void func_080024FC(void *arg0, void *arg1, s32 arg2) {
    register char *r3 asm("r3") = arg0;
    register void *r1 asm("r1") = arg1;
    register s32 r2 asm("r2") = arg2;
    goto start;
    loop:
    r3 += 0xC;
    start:
    if (*(void **)r3 != NULL) goto loop;
    func_080024A4(r3, r1, r2);
}
#endif
