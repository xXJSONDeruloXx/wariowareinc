#if __INCLUDE_LEVEL__ > 0
#include "global.h"
#include "scenes.h"

extern void func_0801429C(u32, u32);
extern void func_08014374(void);

void func_080143A0(void) {
    u8 *ptr;
    ptr = (u8 *)gCurrentSceneData;
    func_0801429C(ptr[0xFD], 1);
    func_08014374();
}
#endif
