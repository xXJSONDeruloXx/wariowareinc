#include "global.h"
#include "src/code_08000f10.h"

u16 func_080CAAEC(void) {
    set_random_seed(*(volatile u16 *)0x04000100);
    return get_random_u16();
}
