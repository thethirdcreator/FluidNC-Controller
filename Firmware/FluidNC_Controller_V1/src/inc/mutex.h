#ifndef __MUTEX_H__
#define __MUTEX_H__

#include "stdlib.h"

typedef enum 
{
    MT_UNAVAILIBLE = 254,
    MT_AVAILIBLE = 255,
    MT_LOCKED = 0,
    MT_UNLOCKED
}_mutex;

void _mt_lock(_mutex *mt);
void _mt_unlock(_mutex *mt);

_mutex _mt_check(_mutex *mt);

#endif