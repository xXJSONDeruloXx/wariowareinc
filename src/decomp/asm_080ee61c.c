#include "global.h"

s32 func_080EE61C(s32 a0, s32 a1) {
    s32 result = a0;
    asm volatile("svc #6" : "+r"(result) : "r"(a1));
    return result;
}