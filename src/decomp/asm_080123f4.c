#if __INCLUDE_LEVEL__ > 0
#include "global.h"

extern void *gCurrentSceneData;
extern const u8 D_083AA568[];
extern void func_08006CE8(u32, const u8 *, u32, u32);

void func_080123F4(void) {
    void **ptr = &gCurrentSceneData;
    void *data = *ptr;
    u16 val = *(u16 *)((u8 *)data + 0x88);
    u32 temp = (u32)val << 0x17;
    u32 r3 = temp >> 0x19;
    if (r3 > 0x20) {
        r3 = 0x20;
    }
    func_08006CE8(0, D_083AA568, 0x20, r3);
}
#endif
