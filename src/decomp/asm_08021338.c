#include "global.h"
#include "src/beatscript.h"

extern void func_080211A4(void);
extern void func_08021294(void);

void func_08021338(void) {
    if (D_03006520 == 0x0A) {
        func_080211A4();
        func_08021294();
    }
}
