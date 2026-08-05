#include "global.h"

extern void func_0800C7FC(void *);
extern u8 D_083FD264[];

void func_08048DC8(void) {
    func_0800C7FC(D_083FD264);
    *(u8 *)((u8 *)gCurrentSceneVariable + 0x6C) = 3;
}
