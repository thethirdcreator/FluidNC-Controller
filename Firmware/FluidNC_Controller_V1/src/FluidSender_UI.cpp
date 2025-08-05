#include "inc/FluidSender_UI.h"

// uint8_t colPins[COLS] = {4, 3, 2, 1};
// uint8_t rowPins[ROWS] = {13, 10, 8, 6, 5};

// char keys[ROWS][COLS] = {
//     {KPD_F1, KPD_F2, KPD_HASH, KPD_AST},
//     {KPD_1, KPD_2, KPD_3, KPD_UP},
//     {KPD_4, KPD_5, KPD_6, KPD_DN},
//     {KPD_7, KPD_8, KPD_9, KPD_ESC},
//     {KPD_LT, KPD_0, KPD_RT, KPD_ENT}};

// Keypad UIkeypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);
// U8G2_ST7920_128X64_1_HW_SPI u8g2(U8G2_R0, 12);

// UI_Context_t mainContext = {MainScreen, NULL};
UI_Context_t mainContext = {MainScreen};
UI_Context_t alarmContext = {AlarmScreen};

// FluidSender_UI_Class UI = {mainDraw, &mainContext, NULL};

void FluidSender_UI_Class::begin(U8G2 *d_driver, Keypad *k_driver)
{
    // UI->keyDriver->addEventListener(UI->context->keypadEventListener);
    this->dispDriver = d_driver;
    this->keyDriver = k_driver;
}

void FluidSender_UI_Class::draw()
{

    switch (CNC.getStatus())
    {
    case FLUID_ALARM:
        this->context = &alarmContext;
        break;
    case FLUID_IDLE:
    case FLUID_JOG:
    case FLUID_MOVING:
    case FLUID_HOMING:
        this->context = &mainContext;
        break;
    default:
        this->context = &mainContext;
        break;
    }

    this->dispDriver->firstPage();
    do
    {
        this->context->screen(this->dispDriver);

    } while (this->dispDriver->nextPage());
}

void MainScreen(U8G2 *driver)
{
    driver->setFont(UI_FONT_NORMAL);
    driver->setCursor(0, 8);
    // driver->print("Main screen");
    driver->print("Status: ");
    driver->print(statusToText(CNC.getStatus()));

    driver->setCursor(V_POS_X, V_POS_Y);
    driver->print("V:");
    driver->print(ota.version());

    driver->setCursor(IP_POS_X, IP_POS_Y);
    if (WiFi.isConnected())
    {
        driver->print("IP:");
        driver->print(WiFi.localIP());
    }
    else
    {
        driver->print("WiFi not connected");
    }

    // Отображать только, когда ввод не нулевой
    if (CNC.inputPos.length())
    {
        // Вывод нового положения
        driver->setCursor(64 - (CNC.inputPos.length() * 4) - driver->getStrWidth("->"), 24 + 5 + 16);
        driver->setCursor(50, 45);
        driver->print("->");
        driver->print(CNC.inputPos);
        driver->print("<-");
    }
    driver->setFont(UI_FONT_BIG);
    driver->setCursor(POS_POS_X, POS_POS_Y);
    driver->print(CNC.x.getRealPos());
}

void AlarmScreen(U8G2 *driver)
{
    driver->setFont(UI_FONT_NORMAL);
    driver->setCursor(0, 8);
    driver->print("ALARM! Hold ESC to reset");

    driver->setCursor(V_POS_X, V_POS_Y);
    driver->print("V:");
    driver->print(ota.version());

    driver->setCursor(IP_POS_X, IP_POS_Y);
    if (WiFi.isConnected())
    {
        driver->print("IP:");
        driver->print(WiFi.localIP());
    }
    else
    {
        driver->print("WiFi not connected");
    }
}

void HomingPendingScreen(U8G2 *driver)
{

    driver->setCursor(0, 8);
    driver->print("Нужен хоуминг");

    driver->setCursor(V_POS_X, V_POS_Y);
    driver->print("V:");
    driver->print(ota.version());

    driver->setCursor(IP_POS_X, IP_POS_Y);
    driver->print("IP:");
    driver->print(WiFi.localIP());
}

void SettingsScreen(U8G2 *driver)
{
}

void FluidNC_Updater(FluidSender_UI_Class *UI)
{

    _LogPrintLn("Checking update availability...");
    _LogPrint("Update status: ");
    _LogPrintLn(ota.checkUpdate());

    UI->dispDriver->firstPage();
    do
    {
        UI->dispDriver->setFont(u8g2_font_unifont_t_cyrillic); // Set Russian font
        UI->dispDriver->setCursor(0, 32);
        UI->dispDriver->print("Поиск обновлений");
    } while (UI->dispDriver->nextPage());

    delay(1000);

    if (ota.checkUpdate())
    {

        Serial.println("New version found!");
        Serial.println("Updating in ...");
        Serial.println("3");
        delay(1000);
        Serial.println("2");
        delay(1000);
        Serial.println("1");
        delay(1000);

        UI->dispDriver->firstPage();
        do
        {
            UI->dispDriver->setFont(u8g2_font_unifont_t_cyrillic); // Set Russian font
            UI->dispDriver->setCursor(0, 32);
            UI->dispDriver->print("Обновление");
        } while (UI->dispDriver->nextPage());

        Serial.println("Performing an update...");
        if (!ota.updateNow())
            Serial.println("\n\nUpdate failed!");
        AutoOTA::Error otaError = ota.getError();
        Serial.println("Error: ");
        Serial.println((uint8_t)otaError);
    }
    else
    {

        Serial.print("Update error: ");
        AutoOTA::Error otaError = ota.getError();
        Serial.println((uint8_t)otaError);

        if (otaError == AutoOTA::Error::NoUpdates)
        {
            UI->dispDriver->firstPage();
            do
            {
                UI->dispDriver->setFont(u8g2_font_unifont_t_cyrillic); // Set Russian font
                UI->dispDriver->setCursor(0, 32);
                UI->dispDriver->print("Обновлений нет");
            } while (UI->dispDriver->nextPage());
            delay(1000);
        }
        else
        {
            UI->dispDriver->firstPage();
            do
            {
                UI->dispDriver->setFont(u8g2_font_unifont_t_cyrillic); // Set Russian font
                UI->dispDriver->setCursor(0, 32);
                UI->dispDriver->print("Ошибка обновления");
            } while (UI->dispDriver->nextPage());
            delay(1000);
        }
    }
}