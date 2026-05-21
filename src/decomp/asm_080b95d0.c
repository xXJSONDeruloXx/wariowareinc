#include "global.h"
#include "types.h"

void func_080B93F0(void);
void func_080B9478(void);

void func_080B95D0(void) {
    u8 *p;
    func_080B93F0();
    p = (u8 *)gCurrentSceneVariable;
    if (p[0x61] == 1) func_080B9478();
}
