#include "global.h"

typedef struct {
    u8 padding0[4];
    u32 field;
    u8 padding8[0x18];
} Func080F2558Entry;

typedef struct {
    u8 padding0[0x18];
    Func080F2558Entry *entries;
} Func080F2558Context;

void func_080F2558(Func080F2558Context *arg0, s32 arg1, u8 arg2) {
    u32 value;
    u32 base;
    u32 offset;
    u32 field;
    u32 mask;
    Func080F2558Entry *entry;

    value = arg2;
    value <<= 24;
    value >>= 24;
    base = (u32)arg0->entries;
    offset = arg1;
    offset <<= 5;
    offset += base;
    value &= 0x7F;
    value <<= 14;
    entry = (Func080F2558Entry *)offset;
    field = entry->field;
    mask = 0xFFE03FFF;
    field &= mask;
    field |= value;
    entry->field = field;
}
