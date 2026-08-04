#include "global.h"

extern void func_080F2F04(void);
extern void func_080F2F34(void);

void func_08002024(s32 arg0) {
    if (arg0 != 0) {
        func_080F2F04();
        return;
    }
    func_080F2F34();
}
