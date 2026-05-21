#include "global.h"
#include "types.h"

void func_080058DC(u32, u32);

void func_08024024(u32 arg0) {
    u8 *p = (u8 *)gCurrentSceneVariable;
    func_080058DC(*(u32 *)(p + 8), arg0);
}
