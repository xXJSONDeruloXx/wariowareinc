#if __INCLUDE_LEVEL__ > 0
#include "global.h"

extern void func_0800247C(void *, void *);

struct Func080024E4Entry {
    void *value;
    u8 padding[8];
};

void func_080024E4(void *arg0, void *arg1) {
    struct Func080024E4Entry *entry;

    entry = (struct Func080024E4Entry *)arg0;
    while (entry->value != NULL) {
        entry++;
    }
    func_0800247C(entry, arg1);
}
#endif
