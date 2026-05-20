#include "global.h"
#include "src/beatscript.h"

extern void func_080215B4(void);
extern void func_080216A4(void);

void func_08021748(void) {
    if (D_03006520 == 0x28) {
        func_080215B4();
        func_080216A4();
    }
}
