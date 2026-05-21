#include "global.h"
#include "types.h"

void func_08026264(u32, u32);

void func_08026224(u32 arg0) {
    func_08026264(10, arg0);
    ((u8 *)gCurrentSceneVariable)[4] |= 0x8;
}
