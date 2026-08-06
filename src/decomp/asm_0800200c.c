#include "global.h"

extern void func_080F2EEC(u32);
extern void func_080F2EF8(u32);

void func_0800200C(s32 arg0, s32 arg1) {
    if (arg0 != 0) {
        if (arg1 != 0) {
            func_080F2EEC((u32)arg0);
            return;
        }
        func_080F2EF8((u32)arg0);
    }
}
