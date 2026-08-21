#if __INCLUDE_LEVEL__ > 0
#include "global.h"
#include "scenes.h"
#include "src/scenes/main_menu.h"
#include "src/audio.h"

extern void scene_set_current_thread(u32);
extern void func_080117A8(s32);
extern void func_08011864(u32);
extern u32 D_083FBBF8;

void func_080118E0(void) {
    u8 *flags;
    u8 val;
    u32 mask;

    scene_set_current_thread(0);
    func_080117A8(D_03006518.unk2);
    func_08011864(D_03006518.unk2);
    flags = (u8 *)gCurrentSceneData + 0xDD;
    val = *flags;
    mask = 2;
    mask = -mask;
    mask = val & mask;
    *flags = mask;
    play_sound((struct SongHeader *)&D_083FBBF8);
}
#endif
