#include "global.h"

typedef struct Func08007F20Node Func08007F20Node;
struct Func08007F20Node {
    void *id;
    u32 data;
    u8 padding[0xC];
    Func08007F20Node *next;
};

extern Func08007F20Node *D_0300485C;

void *func_08007F20(void *arg0) {
    Func08007F20Node *node;

    node = D_0300485C;
    while (node != 0) {
        if (node->id == arg0) {
            return &node->data;
        }
        node = node->next;
    }
    return arg0;
}
