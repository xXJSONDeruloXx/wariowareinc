#include "global.h"
#include "src/code_08000f10.h"
#include "src/scenes/gameplay.h"

extern void func_08024BD4(void);
extern void set_pause_beatscript_scene(u32);
extern void func_08024B54(u16);

void func_080179A8(void) {
    func_08024BD4();
    if (gGameplayData.unk23c != 0xFF) {
        set_pause_beatscript_scene(0);
    }
    func_08024B54(get_random_range(0x10));
}
