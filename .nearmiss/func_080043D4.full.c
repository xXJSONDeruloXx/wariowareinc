#include "global.h"

typedef struct {
    u8 padding[0xA];
    u8 count;
    u8 paddingB;
    u16 *halfwords;
    u8 *bytes;
} Func080043D4Data;

void func_080043D4(Func080043D4Data *arg0) {
    u32 index;
    u32 zero;

    index = 0;
    if (arg0->count != 0) {
        zero = 0;
        do {
            arg0->halfwords[index] = zero;
            arg0->bytes[index] = zero;
            index++;
        } while (index < (arg0->count << 4));
    }
}
