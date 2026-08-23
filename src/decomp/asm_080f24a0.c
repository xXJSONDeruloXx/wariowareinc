#include "global.h"

typedef struct {
    u16 field;
    u8 padding2[0x1E];
} Func080F24A0Entry;

typedef struct {
    u8 padding0[0x18];
    Func080F24A0Entry *entries;
} Func080F24A0Context;

void func_080F24A0(Func080F24A0Context *arg0, s32 arg1, u8 arg2) {
    u32 value;
    u32 base;
    u32 offset;
    u16 mask;
    u32 field;
    Func080F24A0Entry *entry;

    value = arg2;
    value <<= 24;
    value >>= 24;
    base = (u32)arg0->entries;
    offset = arg1;
    offset <<= 5;
    offset += base;
    value &= 0x7F;
    value <<= 2;
    entry = (Func080F24A0Entry *)offset;
    mask = entry->field;
    field = 0xFFFFFE03;
    field &= mask;
    field |= value;
    entry->field = field;
}
