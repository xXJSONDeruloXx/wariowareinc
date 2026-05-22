#if __INCLUDE_LEVEL__ > 0
#include "global.h"
#include "scenes.h"
#include "src/beatscript.h"

void func_080109CC(void) {
    u8 *ptr;
    u8 val;
    register u32 m asm("r0");
    set_pause_beatscript_scene(0);
    ptr = (u8 *)gCurrentSceneData;
    ptr = ptr + 0xDF;
    val = ptr[0];
    m = 3;
    m = -m;
    m = val & m;
    ptr[0] = m;
}
#endif
