#include "global.h"

s32 func_0808EA3C(void) {
    u8 *p = (u8 *)gCurrentSceneVariable;

    if (p[0xBC4] == 0x1B) {
        return 1;
    }
    return 0;
}
