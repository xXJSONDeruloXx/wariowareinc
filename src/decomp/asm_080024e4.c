#if __INCLUDE_LEVEL__ > 0
#include "global.h"

extern void func_0800247C(void *);

void func_080024E4(void *arg0) {
    register char *r2 asm("r2") = arg0;
    goto start;
loop:
    r2 += 0xC;
start:
    if (*(void **)r2 != NULL)
        goto loop;
    func_0800247C(r2);
}
#endif
