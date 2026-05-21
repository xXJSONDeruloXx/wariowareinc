#include "global.h"

extern void func_080B7D40(void);
extern void func_080B7BCC(void);
extern u32 func_080B8178(void);

u8 func_080B7BB4(void) {
    func_080B7D40();
    func_080B7BCC();
    return (u8)func_080B8178();
}
