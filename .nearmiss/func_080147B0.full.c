#include "src/scenes/main_menu.h"

extern s32 func_08011708(void);
extern s32 func_08011698(void);
extern void func_080145D4(void);
extern void func_08015944(s16, void (*)(void));
extern void func_08014428(void);
extern void set_pause_beatscript_scene(u32);

void func_080147B0(void) {
    if (func_08011708() != 0 && gMainMenu.unk13C_2 != 0) {
        func_08015944(gMainMenu.unk1C4, func_08014428);
        gMainMenu.unk13C_2 = 0;
        set_pause_beatscript_scene(0);
    } else if (func_08011698() != 0) {
        func_080145D4();
    }
}
