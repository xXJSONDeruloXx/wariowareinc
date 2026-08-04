#if __INCLUDE_LEVEL__ > 0
#include "global.h"
#include "scenes.h"

extern void func_0800B828(u32, u32, u32);
extern void func_0800BA78(void);
extern u32 D_083ADADC;

void func_0800BB74(void *arg0) {
    u8 *data;

    func_0800B828((u32)arg0, (u32)&D_083ADADC, 0);
    data = (u8 *)gCurrentSceneData;
    *(u32 *)(data + 0x180) = (u32)(data + 0x198);
    *(u16 *)(data + 0x184) = 0x78;
    *(u16 *)(data + 0x186) = 0x40;
    func_0800BA78();
}
#endif
