#include "src/audio.h"
#include "src/code_08000f10.h"
#include "scenes.h"

extern void func_08006C40(u32, u32);
extern u32 func_08016850(void);
extern struct SongHeader D_083FBB44;

void func_08016798(void) {
    if ((func_08016850() != 0) && ((gPressedKeys & 0xB) != 0)) {
        func_08006C40(0x20, 0);
        play_sound(&D_083FBB44);
        *(u16 *)((u8 *)gCurrentSceneData + 0x3A) = 1;
    }
}
