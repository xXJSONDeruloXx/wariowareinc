#if __INCLUDE_LEVEL__ > 0
#include "global.h"
#include "src/code_08000f10.h"
#include "src/audio.h"
#include "src/scenes/gameplay.h"

extern void func_08014D6C(void);

void func_08014DC4(void) {
    if (gPressedKeys & 3) {
        func_08014D6C();
        play_sound((struct SongHeader *)&D_083FBBBC);
    }
}
#endif
