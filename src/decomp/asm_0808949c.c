#include "global.h"
#include "types.h"

void func_080894B4(u32);

void func_0808949C(void) {
    u8 *p = (u8 *)gCurrentSceneVariable;
    func_080894B4((u32)(p + (0xC2 << 1)));
}
