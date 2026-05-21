#include "global.h"

extern void func_0802E020(void *arg0);
extern void func_0802E2A4(void *arg0);

void func_0802E4AC(void *arg0) {
    (*(u32*)((u8*)arg0 + 0x6C))++;
    func_0802E020(arg0);
    func_0802E2A4(arg0);
}
