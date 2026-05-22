#if __INCLUDE_LEVEL__ > 0
#include "global.h"

extern void func_08012274(void);
extern void func_08013184(void);

void func_080118C4(s32 arg0) {
    switch (arg0) {
        case 0:
            func_08012274();
            break;
        case 1:
            func_08013184();
            break;
    }
}
#endif
