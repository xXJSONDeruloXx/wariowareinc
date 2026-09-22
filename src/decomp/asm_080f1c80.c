#include "global.h"

extern u16 D_030068EC;
extern void func_080F1C14(u32 index);
extern void func_080F2BE4(void);

void func_080F1C80(void)
{
    u32 i;

    for (i = 0; i < D_030068EC; i++) {
        func_080F1C14(i);
    }
    func_080F2BE4();
}
