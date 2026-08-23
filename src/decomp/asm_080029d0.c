#include "global.h"

struct Func080029D0Data {
    union {
        u8 byte;
        u16 half;
    } value;
};

void func_080029D0(void *arg0) {
    struct Func080029D0Data *data;
    u32 value;
    u32 mask;

    data = (struct Func080029D0Data *)arg0;
    value = data->value.byte;
    mask = 3;
    mask = -mask;
    mask &= value;
    data->value.byte = mask;
    value = data->value.half;
    mask = 3;
    mask &= value;
    data->value.half = mask;
}
