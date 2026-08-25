#include "global.h"

typedef struct {
    u8 flags;
    u8 padding[7];
    s32 unk8;
    u8 rest[0x10];
} Func0800567CEntry;

typedef struct {
    u32 unk0;
    u32 unk4;
    Func0800567CEntry entries[48];
} Func0800567CState;

extern Func0800567CState D_03000698;

s32 func_0800567C(void) {
    u8 flags;
    s32 index;
    s32 mask;
    s32 invalid;
    Func0800567CEntry *entry;

    D_03000698.unk0 = 0;
    index = 0;
    mask = -2;
    entry = D_03000698.entries;
    invalid = -1;
    do {
        flags = entry->flags;
        flags &= mask;
        entry->flags = flags;
        entry->unk8 = invalid;
        entry++;
        index++;
    } while (index <= 0x2F);
    return flags;
}
