#ifndef __FLUIDSENDER_UI_H__
#define __FLUIDSENDER_UI_H__

#include "inc/FluidNC_CNC.h"
#include "U8g2lib.h"
#include "inc/FluidNC_Updater.h"
#include "inc/FluidSender_Keypad.h"

#define UI_FONT_BIG u8g2_font_spleen12x24_mr
#define UI_FONT_NORMAL u8g2_font_haxrcorp4089_t_cyrillic

// Version
#define V_POS_X (128 - driver->getStrWidth("V:1.X"))
#define V_POS_Y 64

// IP
#define IP_POS_X 0
#define IP_POS_Y 64

// Pos
#define POS_POS_X 20
#define POS_POS_Y 30

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
    Keypad *keyDriver;
    public:
    U8G2 *dispDriver;
    
    void begin(U8G2 *d_driver, Keypad *k_driver);
    // void (*draw)();
    void draw();
    UI_Context_t *context;
};

void FluidNC_Updater(FluidSender_UI_Class *UI);// костыль - сделать отдельным экраном
extern UI_Context_t mainContext;

#endif