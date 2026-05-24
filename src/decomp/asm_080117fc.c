#if __INCLUDE_LEVEL__ > 0
#include "global.h"
#include "src/scenes/main_menu.h"

extern void func_080117A8(s32);
extern void func_0800C77C(u32);
extern void func_0800C7A4(s32);

void func_080117FC(void) {
    s32 i;
    func_080117A8(D_03006518.unk2);
    i = 0;
    do {
        i++;
        func_0800C77C(i);
    } while (i <= 2);
    func_0800C7A4(6);
}
#endif
