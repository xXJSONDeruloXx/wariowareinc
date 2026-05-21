#include "global.h"
#include "types.h"

void func_080AD900(void);

void func_080ADAC0(void) {
    u8 *p = (u8 *)gCurrentSceneVariable;
    if (p[0x25] == 0) func_080AD900();
}
