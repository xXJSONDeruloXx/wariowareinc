#include "global.h"

typedef struct {
    u8 pad0[0x37C];
    u16 field37C;
} Func080D3A60Scene;

extern void func_080D3BE8(void);

u16 *func_080D3A60(void) {
    func_080D3BE8();
    ((Func080D3A60Scene *)gCurrentSceneVariable)->field37C = 1;
    return &((Func080D3A60Scene *)gCurrentSceneVariable)->field37C;
}
