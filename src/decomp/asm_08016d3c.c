#include "global.h"
#include "types.h"
#include "src/lib_sprite.h"

void func_08016D3C(void)
{
    u32 i;

    func_08000F74(NULL);
    func_08003E64();
    for (i = 0; i < 1;)
    {
        i++;
        sprite_id_delete(gSpriteHandler, i);
        func_08001B70(i);
        task_pool_force_cancel_id(i);
        mem_heap_dealloc_with_id(i);
    }
}
