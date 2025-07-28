#ifndef __FLUID_UPDATER_HPP__
#define __FLUID_UPDATER_HPP__

#include <Arduino.h>
#include <U8g2lib.h>
#include <AutoOTA.h>
#include "WiFiList.h"
#include "Debug.hpp"

#define FluidNC_Controller_Ver "1.0"

extern U8G2_ST7920_128X64_1_HW_SPI u8g2;
extern AutoOTA ota;

void WiFi_Check();
void ARDUINO_ISR_ATTR WiFiHandler();
void FluidNC_Updater();

#endif