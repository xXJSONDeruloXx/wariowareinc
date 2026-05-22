#if __INCLUDE_LEVEL__ > 0
#include "global.h"

void func_08002514(void *arg0) {
    register char *r4 asm("r4") = arg0;
    goto start;
loop:
    r4 += 12;
start:
    if (*(void **)r4 != NULL)
        goto loop;
    func_080024D0((u32 *)r4, 0, 0, 0);
}
#endif
