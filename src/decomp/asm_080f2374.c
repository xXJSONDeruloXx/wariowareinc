#include "global.h"

typedef struct {
    u8 padding0[2];
    u16 field;
    u8 padding4[0x1C];
} Func080F2374Entry;

typedef struct {
    u8 padding0[0x18];
    Func080F2374Entry *entries;
} Func080F2374Context;

void func_080F2374(Func080F2374Context *arg0, s32 arg1, u8 arg2) {
    u32 value;
    u32 base;
    u32 offset;
    u16 mask;
    u32 field;
    Func080F2374Entry *entry;

    value = arg2;
    value <<= 24;
    value >>= 24;
    base = (u32)arg0->entries;
    offset = arg1;
    offset <<= 5;
    offset += base;
    value &= 0x7F;
    value <<= 7;
    entry = (Func080F2374Entry *)offset;
    mask = entry->field;
    field = 0xFFFFC07F;
    field &= mask;
    field |= value;
    entry->field = field;
}
