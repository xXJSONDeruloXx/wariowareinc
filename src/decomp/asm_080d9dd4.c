#include "global.h"
#include "types.h"

void func_080D9DF4(u32);
void func_080D9E40(u32);

void func_080D9DD4(void) {
    u8 *p = (u8 *)gCurrentSceneVariable + 0x18;
    func_080D9DF4((u32)p);
    func_080D9E40((u32)p);
}
