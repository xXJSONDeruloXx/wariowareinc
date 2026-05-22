#if __INCLUDE_LEVEL__ > 0
#include "global.h"
#include "scenes.h"

extern void func_0800BC50(void);

void func_0800BC90(void) {
    u8 *ptr;
    s32 val;
    ptr = (u8 *)gCurrentSceneData;
    val = ptr[7];
    val = val << 0x1D;
    if (val >= 0) return;
    func_0800BC50();
}
#endif
