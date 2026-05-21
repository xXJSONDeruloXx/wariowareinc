#include "global.h"

u32 func_080020FC(u32 a0) {
    if (a0 == 0) {
        return 0;
    }
    return *(u32 *)(a0 + 0xC);
}