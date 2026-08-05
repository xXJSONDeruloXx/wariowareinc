#include "global.h"

extern u8 D_083E8448[];
extern u8 func_0800A024(void);

u8 func_080E1F48(void) {
    return D_083E8448[func_0800A024()];
}
