#include "global.h"
#include "graphics.h"
#include "src/audio.h"
#include "src/beatscript.h"

void func_080167D4(void) {
    if (gGraphicsBuffer.unk854_4 != 0) {
        stop_all_soundplayers();
        gBeatscriptScene.threads[0].active = 0;
    }
}
