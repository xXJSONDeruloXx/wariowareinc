#include "global.h"

extern u16 gCurrentKeys;
extern void func_08009EE4(u32);
extern void func_08022938(void);
extern void func_0802295C(void);
extern void func_08022980(void);

void func_08022650(void) {
    func_08022938();
    func_0802295C();
    func_08022980();
    func_08009EE4(((u16)gCurrentKeys >> 8) & 1);
}
