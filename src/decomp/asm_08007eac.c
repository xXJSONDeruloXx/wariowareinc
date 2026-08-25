#include "global.h"

typedef struct Func08007EACNode Func08007EACNode;
struct Func08007EACNode {
    u8 padding[4];
    void *data;
    u8 padding2[0xC];
    Func08007EACNode *next;
};

extern Func08007EACNode *D_0300485C;
extern void mem_heap_dealloc(void *ptr);

void func_08007EAC(void) {
    Func08007EACNode *node;

    node = D_0300485C;
    while (node != 0) {
        Func08007EACNode *next = node->next;
        mem_heap_dealloc(node->data);
        mem_heap_dealloc(node);
        node = next;
    }
    D_0300485C = 0;
}
