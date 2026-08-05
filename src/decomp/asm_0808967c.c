#include "global.h"
#include "scenes.h"

void func_0808967C(void *arg0) {
    u32 base = *(u16 *)((u8 *)gCurrentSceneData + 0x16);
    u32 delta = base << 1;
    u32 value;

    delta += base;
    delta >>= 4;
    value = *(u32 *)((u8 *)arg0 + 0x28);
    value += delta;
    *(u32 *)((u8 *)arg0 + 0x28) = value;
}
