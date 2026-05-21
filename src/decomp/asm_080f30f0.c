#include "global.h"

void func_080F3040(u32, u32, u32);

void func_080F30F0(u32 arg0, u32 arg1) {
    func_080F3040(arg0, 3, (u32)arg1 << 20 >> 16);
}
