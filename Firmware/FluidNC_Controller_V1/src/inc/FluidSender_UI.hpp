#ifndef __FLUIDSENDER_UI_H__
#define __FLUIDSENDER_UI_H__

#include "inc/FluidNC_CNC.hpp"
#include "U8g2lib.h"
#include "inc/FluidNC_Updater.hpp" // в нем обьявлена u8g2 - костыль

// Version
//     u8g2.setCursor(128 - u8g2.getStrWidth("V:1.X"), 64);
#define V_POS_X (128 - u8g2.getStrWidth("V:1.X"))
#define V_POS_Y 64

// IP
//     u8g2.setCursor(0, 64);
#define IP_POS_X 0
#define IP_POS_Y 64

// Pos
//       u8g2.setCursor(40, 24);
#define POS_POS_X 40
#define POS_POS_Y 24

void mainDraw();
void MainScreen();
void AlarmScreen();
void SettingsScreen();

typedef struct 
{
    void (*draw)();
    void (*screenType)();
    U8G2 *driver;
}FluidSender_UI_t;

extern FluidSender_UI_t UI;

#endif