#include "global.h"
#include "types.h"

void func_08088BB4(u32);

void func_08088B9C(void) {
    u8 *p = (u8 *)gCurrentSceneVariable;
    func_08088BB4((u32)(p + (0xA2 << 1)));
}
