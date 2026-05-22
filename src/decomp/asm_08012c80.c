#if __INCLUDE_LEVEL__ > 0
#include "global.h"

extern void *gCurrentSceneData;
extern u32 D_083AA3C4[];
extern u32 save_is_stage_unlocked(u32);
extern u32 func_0800C874(u32);
extern u32 func_080020FC(u32);

void func_08012C80(u32 arg0) {
    register u32 id asm("r4");
    register u32 *table asm("r1");
    register u32 addr asm("r0");
    register u8 *data asm("r1");
    register u32 result asm("r0");

    id = arg0;
    if (save_is_stage_unlocked(arg0) != 0) {
        table = D_083AA3C4;
        addr = id << 2;
        addr += (u32)table;
        result = func_0800C874(*(u32 *)addr);
        result = func_080020FC(result);
        data = gCurrentSceneData;
        data += 0x84;
        *(u32 *)data = result;
    }
}
#endif
