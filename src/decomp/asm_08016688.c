#include "global.h"

extern void func_0800207C(u32, u16);
extern void *gCurrentSceneData;

void func_08016688(void) {
    func_0800207C((u32)gBeatscriptScene.musicPlayer,
                  *(u16 *)((u8 *)gCurrentSceneData + 0x1D0));
}
