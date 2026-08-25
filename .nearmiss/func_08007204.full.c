#include "global.h"

typedef struct {
    s16 field_0;
    u16 field_2;
    u16 field_4;
    s16 field_6;
    s32 field_8;
    s32 field_C;
} Func08007204Data;

extern void *gSpriteHandler;
extern void sprite_set_x_y(void *, s32, s32, s32);

s32 func_08007204(Func08007204Data *arg0) {
    s32 x;
    s32 y;
    s32 scale;

    x = arg0->field_8;
    if (x < 0) {
        x = -x;
    }
    y = arg0->field_C;
    if (y < 0) {
        y = -y;
    }
    scale = arg0->field_6;
    x = (scale * x) >> 8;
    y = (scale * y) >> 8;
    if (x <= 0xFF) {
        x = 0;
    }
    if (y <= 0xFF) {
        y = 0;
    }
    if (arg0->field_8 < 0) {
        x = -x;
    }
    if (arg0->field_C < 0) {
        y = -y;
    }
    arg0->field_8 = x;
    arg0->field_C = y;
    sprite_set_x_y(gSpriteHandler, arg0->field_0,
                   (s16)((x >> 8) + arg0->field_2),
                   (s16)((y >> 8) + arg0->field_4));
    if ((x | y) == 0) {
        return 1;
    }
    return 0;
}
