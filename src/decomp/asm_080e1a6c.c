#include "global.h"
#include "types.h"

void func_080E1A6C(void *arg0) {
    u8 *p = arg0;
    *(u32 *)(p + 4) += *(u32 *)(p + 0x24);
    *(u32 *)(p + 8) += *(u32 *)(p + 0x28);
}
