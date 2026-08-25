#include "global.h"

u8 *func_08007AF8(u8 *arg0, u8 *arg1) {
    u8 *source;
    u8 *result;
    u8 *cursor;

    if (*arg0 == 0) {
        source = arg1;
        result = arg0;
    } else {
        cursor = arg0;
        do {
            cursor++;
        } while (*cursor != 0);
        source = arg1;
        result = cursor;
    }
    while (*source != 0) {
        *result = *source;
        source++;
        result++;
    }
    *result = 0;
    return arg0;
}
