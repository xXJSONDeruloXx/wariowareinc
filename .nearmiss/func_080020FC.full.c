#include "global.h"

u32 func_080020FC(void *arg0) {
    if (arg0 != NULL) {
        return *(u32 *)((u8 *)arg0 + 0xC);
    }
    return 0;
}
