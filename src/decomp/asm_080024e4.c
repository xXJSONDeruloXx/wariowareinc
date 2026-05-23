#if __INCLUDE_LEVEL__ > 0
#include "global.h"

extern void func_0800247C(void *, void *);

void func_080024E4(void *arg0, void *arg1) {
    register char *r2 asm("r2") = arg0;
    register void *r1 asm("r1") = arg1;
    goto start;
    loop:
    r2 += 0xC;
    start:
    if (*(void **)r2 != NULL) goto loop;
    func_0800247C(r2, r1);
}
#endif
