#include "global.h"

void func_0800397C(void *arg0, u8 val) {
    u8 **pp = (u8 **)arg0;
    u8 *p = *pp;
    *p = val;
    *pp = p + 1;
}