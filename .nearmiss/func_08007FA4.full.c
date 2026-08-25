#include "global.h"

struct Func08007FA4Input {
    u32 field0;
    u8 padding4[8];
    u8 enabled;
};

struct Func08007FA4Entry {
    u8 padding0[4];
    s32 field4;
};

extern struct Func08007FA4Entry *func_08007F48(struct Func08007FA4Input *arg0);
extern void func_08007E8C(s32 arg0, s32 arg1);

void func_08007FA4(struct Func08007FA4Input *arg0) {
    struct Func08007FA4Entry *entry;

    entry = func_08007F48(arg0);
    if (entry != 0) {
        func_08007E8C(arg0->field0, entry->field4);
    }
}
