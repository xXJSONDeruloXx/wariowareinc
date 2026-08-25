#include "global.h"

typedef struct Func08007EDCNode Func08007EDCNode;
struct Func08007EDCNode {
    void *id;
    void *data;
    u8 padding[0xC];
    Func08007EDCNode *next;
};

extern Func08007EDCNode *D_0300485C;
extern void mem_heap_dealloc(void *ptr);

void func_08007EDC(void *id) {
    Func08007EDCNode *node;
    Func08007EDCNode *previous;

    node = D_0300485C;
    if (node != 0) {
        do {
            if (node->id == id) {
                if (node == D_0300485C) {
                    D_0300485C = node->next;
                } else {
                    previous->next = node->next;
                }
                mem_heap_dealloc(node->data);
                mem_heap_dealloc(node);
                break;
            }
            previous = node;
            node = node->next;
        } while (node != 0);
    }
}
