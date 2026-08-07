#include "global.h"
#include "src/scenes/gameplay.h"

struct Func08025174Table {
    u8 pad0[0x58];
    u32 first[2];
    u32 second[2];
};

void func_08025174(s32 index, s32 value1, s32 value2) {
    struct Func08025174Table *table;

    table = (struct Func08025174Table *)D_03006524;
    table->first[index] = value1;
    table->second[index] = value2;
}
