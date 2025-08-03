#include "inc/FluidSender_UI.h"

uint8_t colPins[COLS] = {4, 3, 2, 1};
uint8_t rowPins[ROWS] = {13, 10, 8, 6, 5};

char keys[ROWS][COLS] = {
    {KPD_F1, KPD_F2, KPD_HASH, KPD_AST},
    {KPD_1, KPD_2, KPD_3, KPD_UP},
    {KPD_4, KPD_5, KPD_6, KPD_DN},
    {KPD_7, KPD_8, KPD_9, KPD_ESC},
    {KPD_LT, KPD_0, KPD_RT, KPD_ENT}};

Keypad UIkeypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);
// U8G2_ST7920_128X64_1_HW_SPI u8g2(U8G2_R0, 12);  

// UI_Context_t mainContext = {MainScreen, NULL};  
UI_Context_t mainContext = {MainScreen};      

// FluidSender_UI_t UI = {mainDraw, &mainContext, NULL};

void UIbeginCustom(FluidSender_UI_t *UI)
{
    // UI->keyDriver->addEventListener(UI->context->keypadEventListener);
    UI->dispDriver = &u8g2;
    UI->draw = mainDraw;

}

void mainDraw(FluidSender_UI_t *UI)
{
    switch (CNC.getStatus())
    {
    case FLUID_ALARM:
        // UI.screenType = AlarmScreen;
        break;
    case FLUID_IDLE:
    case FLUID_JOG:
    case FLUID_MOVING:
    case FLUID_HOMING:
        // UI.screenType = MainScreen;
        break;
    default:
        break;
    }

    u8g2.firstPage();
    do
    {
        UI->context->screen(UI->dispDriver);

    } while (u8g2.nextPage());
}

void MainScreen(U8G2 *u8g2)
{

    u8g2->setCursor(V_POS_X, V_POS_Y);
    u8g2->print("V:");
    u8g2->print(ota.version());

    u8g2->setCursor(IP_POS_X, IP_POS_Y);
    u8g2->print("IP:");
    u8g2->print(WiFi.localIP());
}

void AlarmScreen()
{
}

void SettingsScreen()
{
}