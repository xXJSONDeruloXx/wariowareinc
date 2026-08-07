#include "global.h"

typedef struct {
    u8 value;
    u8 pad0[0x1F];
} Func08022070Entry;

typedef struct {
    u8 pad0[0x10];
    Func08022070Entry *entries;
} Func08022070Scene;

void func_08022070(s32 arg0) {
    Func08022070Scene *scene = (Func08022070Scene *)gCurrentSceneVariable;
    Func08022070Entry *entry = scene->entries;
    u32 count = 0;

    do {
        entry->value = (u8)arg0;
        count++;
        entry++;
    } while (count <= 6);
}
