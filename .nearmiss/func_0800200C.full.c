#include "global.h"

extern void func_080F2EEC(void);
extern void func_080F2EF8(void);

void func_0800200C(s32 arg0, s32 arg1) {
    if (arg0 != 0) {
        if (arg1 != 0) {
            func_080F2EEC();
            return;
        }
        func_080F2EF8();
    }
}
