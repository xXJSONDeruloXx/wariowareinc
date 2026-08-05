#include "global.h"

extern void func_08017054(s16);

void func_08017080(u32 arg0) {
    func_08017054(((s16 *)gCurrentSceneSpritePool)[arg0]);
}
