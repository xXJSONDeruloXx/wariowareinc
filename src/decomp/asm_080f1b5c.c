#include "global.h"
#include "types.h"

typedef struct { u8 padding[8]; u32 value; } Func080F1B5CInner;
typedef struct {
    u8 padding0[0xC];
    Func080F1B5CInner *inner;
    u8 padding10[0xF];
    u8 scale;
} Func080F1B5COuter;

u32 func_080F1B5C(Func080F1B5COuter *arg0) {
    u32 value;
    u32 factor;
    u32 result;

    value = arg0->inner->value;
    value <<= 11;
    value >>= 25;
    factor = arg0->padding0[1];
    factor <<= 25;
    factor >>= 25;
    factor *= value;
    result = arg0->scale;
    result *= factor;
    result >>= 14;
    return result;
}
