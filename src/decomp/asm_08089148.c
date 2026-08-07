#include "global.h"

typedef struct {
    u8 pad0[0x40];
} Func08089148Entry;

extern void func_08089164(Func08089148Entry *entry);

void func_08089148(Func08089148Entry *entry) {
    u32 count = 0;

    do {
        func_08089164(entry);
        count++;
        entry++;
    } while (count <= 1);
}
