#if __INCLUDE_LEVEL__ > 0
#include "global.h"
#include "types.h"

extern void *gCurrentSceneData;
extern void func_0800B828(u32, u32, u32);
extern void func_0800BA78(void);

void func_0800BBCC(u32 arg0, u32 arg1, u32 arg2, u32 arg3, u32 arg4) {
    u8 *data;
    u16 value3 = (u16)arg3;
    u16 value4 = (u16)arg4;

    func_0800B828(arg0, arg1, arg2);
    data = (u8 *)gCurrentSceneData;
    *(u32 *)(data + 0x180) = (u32)(data + 0x198);
    *(u16 *)(data + 0x184) = value3;
    *(u16 *)(data + 0x186) = value4;
    func_0800BA78();
}
#endif
