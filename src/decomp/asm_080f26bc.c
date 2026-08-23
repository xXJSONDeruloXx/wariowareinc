#include "global.h"

typedef struct {
    u8 padding0[0x1D];
    u8 first;
    u8 second;
} Func080F26BCEntry;

typedef struct {
    u8 padding0[0x18];
    Func080F26BCEntry *entries;
} Func080F26BCContext;

void func_080F26BC(Func080F26BCContext *arg0, s32 arg1, u32 arg2) {
    u32 value;
    u32 base;
    u32 base2;
    u32 offset;
    Func080F26BCEntry *entry;

    value = arg2;
    value <<= 24;
    value >>= 24;
    base = (u32)arg0->entries;
    offset = arg1;
    offset <<= 5;
    entry = (Func080F26BCEntry *)(offset + base);
    entry->first = value;
    base2 = (u32)arg0->entries;
    offset += base2;
    entry = (Func080F26BCEntry *)offset;
    entry->second = value;
}
