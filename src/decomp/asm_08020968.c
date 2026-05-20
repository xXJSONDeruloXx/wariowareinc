#include "global.h"
#include "src/beatscript.h"

extern void func_080208DC(void);

void func_08020968(void) {
    if (D_03006520 == 0x28)
        func_080208DC();
}
