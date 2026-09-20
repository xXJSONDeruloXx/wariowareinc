#include "global.h"
s32 func_080F28C4(u32 arg0){s32 var_r0;if(arg0==0)return 0;var_r0=0x800-((u32)(0x400000U/arg0)>>5);if(var_r0<0)var_r0=0;if(var_r0>0x7FF)var_r0=0x7FF;return var_r0;}
