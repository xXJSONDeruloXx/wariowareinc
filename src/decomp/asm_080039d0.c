#include "global.h"

struct Func080039D0Bytes {
    u8 byte0;
    u8 byte1;
    u8 byte2;
    u8 byte3;
};

u32 func_080039D0(u8 **cursor) {
    struct Func080039D0Bytes *base;

    base = (struct Func080039D0Bytes *)(*cursor - 4);
    *cursor = (u8 *)base;
    return base->byte0 | (base->byte1 << 8) | (base->byte2 << 16) | (base->byte3 << 24);
}
