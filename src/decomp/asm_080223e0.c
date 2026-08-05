#include "global.h"
#include "scenes.h"

extern void func_08022304(u16);

void func_080223E0(void) {
    func_08022304(*(u16 *)((u8 *)gCurrentSceneData + 0x17C));
}
