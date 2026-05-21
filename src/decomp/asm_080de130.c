#include "global.h"
#include "types.h"

void func_080DDFC0(void);

void func_080DE130(void) {
    u8 *p = (u8 *)gCurrentSceneVariable;
    if (p[4] == 0) func_080DDFC0();
}
