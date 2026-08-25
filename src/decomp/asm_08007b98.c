#include "global.h"

extern u8 D_083A4B10[];

void func_08007B98(u8 *output, u32 value) {
    u32 number;
    u32 digits;
    u32 index;
    u32 digit;
    u8 *pair;

    digits = 1;
    number = value;
    while (number > 9) {
        digits++;
        number /= 10;
    }
    output += digits * 2;
    *output = 0;
    index = 0;
    while (index < digits) {
        digit = value % 10;
        pair = D_083A4B10 + digit * 2;
        output -= 2;
        output[0] = pair[0];
        output[1] = pair[1];
        value /= 10;
        index++;
    }
}
