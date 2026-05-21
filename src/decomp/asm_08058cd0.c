#include "global.h"
#include "types.h"

void func_080589A4(void);

void func_08058CD0(void) {
    func_080589A4();
    ((u8 *)gCurrentSceneVariable)[0x1F] = 0;
    ((u8 *)gCurrentSceneVariable)[0x1E] = 0;
}
