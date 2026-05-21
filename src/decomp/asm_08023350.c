#include "global.h"
#include "types.h"

void func_080058DC(u32, u32);

void func_08023350(u32 arg0) {
    u8 *p = (u8 *)gCurrentSceneVariable;
    func_080058DC(*(u32 *)(p + 0x14), arg0);
}
