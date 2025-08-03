#ifndef __FLUIDSENDER_UI_H__
#define __FLUIDSENDER_UI_H__

#include "inc/FluidNC_CNC.h"
#include "U8g2lib.h"
#include "inc/FluidNC_Updater.h" // в нем обьявлена u8g2 - костыль
#include "inc/FluidSender_Keypad.h"

// Version
//     u8g2.setCursor(128 - u8g2.getStrWidth("V:1.X"), 64);
#define V_POS_X (128 - u8g2->getStrWidth("V:1.X"))
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
void MainScreen(U8G2 *u8g2);
void AlarmScreen();
void SettingsScreen();



typedef struct
{
    void (*screen)(U8G2 *u8g2);
    // void (*keypadEventListener)(KeypadEvent key);

}UI_Context_t;

typedef struct 
{
    void (*draw)(void*);
    UI_Context_t *context;
    U8G2 *dispDriver;
    Keypad *keyDriver;
}FluidSender_UI_t;

// extern FluidSender_UI_t UI;

void UIbeginCustom(FluidSender_UI_t *UI);
void mainDraw(FluidSender_UI_t *UI);

#endif