#include "global.h"
#include "types.h"

void func_080DA148(u32);

void func_080DA130(void) {
    u8 *p = (u8 *)gCurrentSceneVariable;
    func_080DA148((u32)(p + (0xC0 << 1)));
}
