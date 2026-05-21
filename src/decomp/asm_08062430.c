#include "global.h"
#include "types.h"

void func_080089D8(u32, u32);
void func_0800A270(void);

void func_08062430(void) {
    func_080089D8(0x11, *(u32 *)((u8 *)gCurrentSceneVariable + (0xBD << 4)));
    func_0800A270();
}
