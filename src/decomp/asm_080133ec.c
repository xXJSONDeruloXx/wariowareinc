#if __INCLUDE_LEVEL__ > 0
#include "global.h"
#include "src/scenes/main_menu.h"

extern void scene_set_current_thread(u32);
extern void func_08013AF4(void);
extern void func_08013A94(void);
extern void func_08013B94(void);
extern void func_08013C60(void);
extern void *gCurrentSceneData;

void func_080133EC(void) {
    register u8 *ptr asm("r1");
    register u8 *data asm("r1");
    register u32 value asm("r2");
    register u32 mask asm("r0");

    scene_set_current_thread(0);
    func_08013AF4();
    func_08013A94();
    func_08013B94();
    ptr = (u8 *)&D_03006518;
    ptr[1] = 3;
    func_08013C60();
    data = gCurrentSceneData;
    data += 0xDD;
    value = *data;
    mask = 2;
    mask = -mask;
    mask &= value;
    *data = mask;
}
#endif
