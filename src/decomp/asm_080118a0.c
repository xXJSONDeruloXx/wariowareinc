#if __INCLUDE_LEVEL__ > 0
#include "global.h"

extern void func_08012350(void);
extern void func_08013264(void);
extern void func_080141C8(void);

void func_080118A0(u32 arg0) {
    switch (arg0) {
    case 0:
        func_08012350();
        break;
    case 1:
        func_08013264();
        break;
    case 2:
        func_080141C8();
        break;
    }
}
#endif
