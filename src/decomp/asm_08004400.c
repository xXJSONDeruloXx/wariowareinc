#include "global.h"

extern s32 func_08004770(u8 *);

s32 func_08004400(u8 *arg0) {
    u8 *cursor;
    s32 count;

    cursor = arg0;
    count = 0;
    while (*cursor != 0) {
        if (*cursor != 0x2E) {
            if (*cursor != 0x3A) {
                if (func_08004770(cursor) == 0) {
                    count++;
                }
            }
        }
        cursor += 2;
    }
    return count;
}
