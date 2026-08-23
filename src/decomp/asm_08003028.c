#include "global.h"

typedef struct {
    u32 word0;
    u8 padding[4];
} Func08003028Record;

extern void func_08002FC0(void *, void *);

void func_08003028(void *arg0, void *arg1) {
    Func08003028Record *cursor = arg0;

    goto check;
loop:
    cursor++;
check:
    if (cursor->word0 != 0) goto loop;
    func_08002FC0(cursor, arg1);
}
