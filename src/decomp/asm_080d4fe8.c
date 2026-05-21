#include "global.h"
#include "types.h"

void func_080D2F10(void);

void func_080D4FE8(void) {
    u8 *p = (u8 *)gCurrentSceneVariable + 8;
    func_080D2F10();
    p[0x1E] = 6;
}
