#include "global.h"
#include "src/beatscript.h"

extern void func_080213AC(void);
extern void func_0802149C(void);

void func_08021540(void) {
    if (D_03006520 == 0x14) {
        func_080213AC();
        func_0802149C();
    }
}
