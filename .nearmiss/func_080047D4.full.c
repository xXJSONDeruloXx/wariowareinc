#include "global.h"

extern u8 *D_083A49E8;

u8 *func_080047D4(u8 *arg0) {
    return D_083A49E8 + ((*arg0 - 0x61) * 2);
}
