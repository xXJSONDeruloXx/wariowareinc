#include "global.h"

u8 *func_08007AD4(u8 *destination, const u8 *source, u32 limit) {
    u8 *result;
    u32 count;
    u8 value;

    result = destination;
    count = 0;
    while (1) {
        value = *source;
        if (value == 0) {
            break;
        }
        if (count >= limit) {
            break;
        }
        *destination = value;
        source++;
        destination++;
        count++;
    }
    return result;
}
