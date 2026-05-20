#include "global.h"

u16 func_08006734(void *arg0) {
    u16 val = (u16)*(volatile u16 *)((u8 *)arg0 + 0x1C);
    return (u16)(((u32)val << 20) >> 20);
}