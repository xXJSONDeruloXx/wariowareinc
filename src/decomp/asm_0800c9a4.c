#include "global.h"
#include "scenes.h"

extern s16 ticks_to_frames();

void func_0800C9A4(void) {
    *(s16 *)((u8 *)gCurrentSceneData + 0x27E) = ticks_to_frames();
}
