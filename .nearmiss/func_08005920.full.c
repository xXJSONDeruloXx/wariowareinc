#include "global.h"
#include "types.h"

extern u8 D_030006A0;

u32 func_08005920(s32 arg0) {
    u32 count;
    u8 *task;
    u32 flagMask;
    u32 stateMask;

    if (arg0 < 0) {
        return 0;
    }

    count = 0;
    task = &D_030006A0;
    flagMask = 1;
    stateMask = 0xFFFE;
    for (;;) {
        if ((task[0] & flagMask) != 0 && *(s32 *)(task + 8) == arg0) {
            if ((*(u16 *)task & stateMask) == 0) {
                return 1;
            }
            return 2;
        }
        count += 1;
        task += 0x1C;
        if (count > 0x2F) {
            return 0;
        }
    }
}
