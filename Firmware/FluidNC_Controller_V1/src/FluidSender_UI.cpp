#include "inc/FluidSender_UI.hpp"

FluidSender_UI_t UI = {mainDraw, MainScreen};

void mainDraw()
{
    switch (CNC.getStatus())
    {
    case FLUID_ALARM:
        UI.screenType = AlarmScreen;
        break;
    case FLUID_IDLE:
    case FLUID_JOG:
    case FLUID_MOVING:
    case FLUID_HOMING:
        UI.screenType = MainScreen;
        break;
    default:
        break;
    }

    u8g2.firstPage();
    do
    {
        UI.screenType();

    } while (u8g2.nextPage());
}

void MainScreen()
{
    u8g2.setCursor(V_POS_X, V_POS_Y);
    u8g2.print("V:");
    u8g2.print(ota.version());

    u8g2.setCursor(IP_POS_X, IP_POS_Y);
    u8g2.print("IP:");
    u8g2.print(WiFi.localIP());
}

void AlarmScreen()
{
}

void SettingsScreen()
{
}