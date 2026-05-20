#include "global.h"

void func_08062F00(void *arg0) {
    u32 *fields = (u32 *)arg0;
    u32 tmp = fields[0x2C / 4];
    tmp += 0x40;
    fields[0x2C / 4] = tmp;
    fields[0xC / 4] += tmp;
}