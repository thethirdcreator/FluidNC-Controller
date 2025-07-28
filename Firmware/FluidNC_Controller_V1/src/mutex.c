#include "mutex.h"

void _mt_lock(_mutex *mt)
{
    *mt = MT_LOCKED;
}

void _mt_unlock(_mutex *mt)
{
    *mt = MT_UNLOCKED;
}

_mutex _mt_check(_mutex *mt)
{
    return *mt;
}