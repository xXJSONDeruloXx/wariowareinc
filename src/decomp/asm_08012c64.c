#if __INCLUDE_LEVEL__ > 0
#include "global.h"
#include "src/scenes/main_menu.h"

extern void func_08012C18(u8);
extern void func_08015A88(void);

void func_08012C64(void) {
    if (D_03006518.unk1 == 1) {
        func_08012C18(D_03006518.unk0);
        func_08015A88();
    }
}
#endif
