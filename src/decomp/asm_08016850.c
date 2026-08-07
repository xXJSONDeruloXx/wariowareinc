#include "global.h"
#include "graphics.h"
#include "scenes.h"

u32 func_08016850(void) {
    if ((*(u8 *)((u8 *)gCurrentSceneData + 8) == 0) || (gGraphicsBuffer.unk854_2 != 0)) {
        return 0;
    }
    return 1;
}
