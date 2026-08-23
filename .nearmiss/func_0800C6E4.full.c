#include "global.h"

typedef struct {
    u8 pad0[0xC];
    u16 *fieldC;
} Func0800C6E4Node;

void func_0800C6E4(Func0800C6E4Node **nodes) {
    if (*nodes != 0) {
        s32 value = -1;

        do {
            *(*nodes)->fieldC = value;
            nodes++;
        } while (*nodes != 0);
    }
}
