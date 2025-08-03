#ifndef __FLUID_UPDATER_HPP__
#define __FLUID_UPDATER_HPP__

#include <Arduino.h>
#include <U8g2lib.h>
#include <AutoOTA.h>
#include "inc/WiFiList.h"
#include "inc/Debug.h"

#define FluidNC_Controller_Ver "1.0"
#define WiFi_Check_Freq 10000 // ms

extern AutoOTA ota;

void WiFi_Check();
void ARDUINO_ISR_ATTR WiFiHandler();
void FluidNC_Updater();

#endif