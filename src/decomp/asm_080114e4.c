#if __INCLUDE_LEVEL__ > 0
#include "global.h"
#include "scenes.h"

extern void func_08015A4C(void);
extern void func_080115DC(void);
extern void func_08003A70(void *);
extern void func_08015DBC(void);

void main_menu_scene_paused(void) {
    u8 *ptr;
    func_08015A4C();
    func_080115DC();
    ptr = (u8 *)gCurrentSceneData;
    func_08003A70(ptr + 0x10);
    func_08015DBC();
}
#endif
