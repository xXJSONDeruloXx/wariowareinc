#include "global.h"

extern void func_08020968(void);
extern void func_08009EE4(u32);
extern u16 gCurrentKeys;

void func_080203F8(void) {
    func_08020968();
    func_08009EE4(((u16)gCurrentKeys >> 8) & 1);
}
