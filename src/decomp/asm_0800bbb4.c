#if __INCLUDE_LEVEL__ > 0
#include "global.h"

extern u32 get_current_language(void);
extern void func_0800BB74(void *);

void func_0800BBB4(u32 *arg0) {
    func_0800BB74(arg0[get_current_language()]);
}
#endif
