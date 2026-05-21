#include "global.h"
#include "types.h"

void func_080C9904(u32, u32);

void func_080C98EC(void) {
    u8 *p = (u8 *)gCurrentSceneVariable;
    func_080C9904((u32)(p + 0xAC), (u32)(p + 0xD0));
}
