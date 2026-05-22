#if __INCLUDE_LEVEL__ > 0
#include "global.h"

typedef struct {
    u8 pad[0x20];
    u32 field_20;
} Struct_080EF998;

u32 func_080EF998(Struct_080EF998 *arg0) {
    u32 val;
    
    val = arg0->field_20;
    val = val + 1;
    arg0->field_20 = val;
    if (val == 0) {
        arg0->field_20 = 0x100;
    }
    return arg0->field_20;
}

__attribute__((section(".text"))) const u8 _padding_func_080EF998[] = { 0x00, 0x00 };
#endif
