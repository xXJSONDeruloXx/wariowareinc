#if __INCLUDE_LEVEL__ > 0
#include "global.h"
#include "types.h"
#include "scenes.h"

extern void func_080140C0(void);

/* Scene-flag setter: case 0 / case 1 set bits in the flag byte at
   gCurrentSceneData+0xDD, case 2 delegates to func_080140C0. The byte lives
   past the typed part of the scene record, so the offset stays as bounded,
   visible layout evidence. */
void func_08011864(u32 arg) {
    switch (arg) {
    case 0:
        *(u8 *)((u8 *)gCurrentSceneData + 0xDD) |= 4;
        break;
    case 1:
        *(u8 *)((u8 *)gCurrentSceneData + 0xDD) |= 0x10;
        break;
    case 2:
        func_080140C0();
        break;
    }
}
#endif
