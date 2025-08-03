#ifndef __FLUIDSENDER_UI_H__
#define __FLUIDSENDER_UI_H__

#include "inc/FluidNC_CNC.h"
#include "U8g2lib.h"
#include "inc/FluidNC_Updater.h" // в нем обьявлена u8g2 - костыль
#include "inc/FluidSender_Keypad.h"

// Version
#define V_POS_X (128 - driver->getStrWidth("V:1.X"))
#define V_POS_Y 64

// IP
#define IP_POS_X 0
#define IP_POS_Y 64

// Pos
#define POS_POS_X 40
#define POS_POS_Y 24

void MainScreen(U8G2 *driver);
void AlarmScreen(U8G2 *driver);
void SettingsScreen(U8G2 *driver);



typedef struct
{
    void (*screen)(U8G2 *driver);
    // void (*keypadEventListener)(KeypadEvent key);

}UI_Context_t;

class FluidSender_UI_Class
{
    private:
    U8G2 *dispDriver;
    Keypad *keyDriver;
    public:

    void begin(U8G2 *d_driver, Keypad *k_driver);
    // void (*draw)();
    void draw();
    UI_Context_t *context;
};

extern UI_Context_t mainContext;

#endif