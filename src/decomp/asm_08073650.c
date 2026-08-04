#include "global.h"

extern void func_08072048(void);
extern s32 func_08073540(void);

s32 func_08073650(void) {
    func_08072048();
    return func_08073540();
}

__attribute__((section(".text"))) const u16 _padding_08073650 = 0;
