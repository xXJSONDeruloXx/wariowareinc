#include "global.h"

void func_08062DCC(void *arg0) {
    *(u32*)((u8*)arg0 + 8) += *(u32*)((u8*)arg0 + 0x28);
    *(u32*)((u8*)arg0 + 0xC) += *(u32*)((u8*)arg0 + 0x2C);
}
