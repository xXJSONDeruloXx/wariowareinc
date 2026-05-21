#include "global.h"

void func_080F2EEC(void);
void func_080F2EF8(void);

void func_0800200C(u8 a0, u8 a1) {
    if (a0 == 0) {
        func_080F2EF8();
    } else if (a1 != 0) {
        func_080F2EEC();
    }
}