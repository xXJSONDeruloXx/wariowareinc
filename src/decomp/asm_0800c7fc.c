#include "global.h"

extern u32 play_sound(void);
extern u16 func_0800A044(void);
extern void func_08002038(u32, u16);

s32 func_0800C7FC(void) {
    s32 value = (s32)play_sound();
    func_08002038((u32)value, func_0800A044());
    return value;
}
