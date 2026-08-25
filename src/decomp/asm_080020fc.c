#include "global.h"

struct Func080020FCDataV2 {
    u8 padding0[0xC];
    u32 fieldC;
};

u32 func_080020FC(struct Func080020FCDataV2 *arg0) {
    if (arg0 == 0) {
        return 0;
    }
    return arg0->fieldC;
}
