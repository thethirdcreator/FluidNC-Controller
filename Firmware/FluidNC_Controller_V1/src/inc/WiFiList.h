#ifndef __WIFILIST_H__
#define __WIFILIST_H__

#include <Arduino.h>

typedef struct
{
    char ssid[32];
    char password[32];

} WiFiList_t;

extern WiFiList_t WiFiList[2];

#endif
