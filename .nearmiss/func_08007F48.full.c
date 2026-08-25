#include "global.h"

struct Func08007F48Resource {
    u8 padding0[4];
    u16 size;
};

struct Func08007F48Input {
    struct Func08007F48Resource *resource;
    u32 field4;
    u16 field8;
    u16 fieldA;
    u8 enabled;
};

struct Func08007F48Entry {
    struct Func08007F48Input *input;
    void *data;
    u32 field8;
    u16 fieldC;
    u16 fieldE;
    u8 field10;
    u8 padding11[3];
    struct Func08007F48Entry *next;
};

extern struct Func08007F48Entry *D_0300485C;
extern void *mem_heap_alloc(u32 size);

struct Func08007F48Entry *func_08007F48(struct Func08007F48Input *arg0) {
    struct Func08007F48Entry *entry;

    if (arg0->enabled == 0) {
        return 0;
    }
    entry = D_0300485C;
    while (entry != 0) {
        if (entry->input == arg0) {
            return 0;
        }
        entry = entry->next;
    }
    entry = mem_heap_alloc(0x18);
    entry->input = arg0;
    entry->next = D_0300485C;
    D_0300485C = entry;
    entry->data = mem_heap_alloc(arg0->resource->size << 1);
    entry->field8 = arg0->field4;
    entry->fieldC = arg0->field8;
    entry->fieldE = arg0->fieldA;
    entry->field10 = 0;
    return entry;
}
