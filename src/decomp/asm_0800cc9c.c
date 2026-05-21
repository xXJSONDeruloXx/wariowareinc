#include "global.h"

extern u32 func_0800A038(void);
extern void func_0800CC4C(u32 arg0, u32 arg1, u32 arg2);

void func_0800CC9C(u32 arg0, u32 arg1) {
    func_0800CC4C(func_0800A038(), arg0, arg1);
}
