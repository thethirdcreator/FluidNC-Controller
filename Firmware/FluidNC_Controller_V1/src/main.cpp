#include <Arduino.h>
#include <U8g2lib.h> // Драйвер дисплея
#include <AutoOTA.h> // Обновление по воздуху
#include <WiFi.h>
#include <math.h>
// #include <menu.h> // Для отображения меню на дисплее и в Serial
// #include <menuIO/chainStream.h>
// #include <menuIO/u8g2Out.h>
// #include <menuIO/serialOut.h>
// #include <menuIO/keypadIn.h>
// #include <menuIO/serialIn.h>

#include "inc/WiFiList.h"
#include "inc/FluidSender_Keypad.h"
#include "inc/FluidNC_Cmd.h"

#include "inc/FluidNC_Updater.h"

#include "inc/FluidNC_CNC.h"
#include "inc/FluidNC_Parser.h"
#include "inc/global.h"
#include "inc/FluidSender_UI.h"
#include "inc/Debug.h"

//=========================
// Firmware version
//=========================

//=========================
// Defines
//=========================

#define ROWS 5
#define COLS 4

uint8_t colPins[COLS] = {4, 3, 2, 1};
uint8_t rowPins[ROWS] = {13, 10, 8, 6, 5};

char keys[ROWS][COLS] = {
    {KPD_F1, KPD_F2, KPD_HASH, KPD_AST},
    {KPD_1, KPD_2, KPD_3, KPD_UP},
    {KPD_4, KPD_5, KPD_6, KPD_DN},
    {KPD_7, KPD_8, KPD_9, KPD_ESC},
    {KPD_LT, KPD_0, KPD_RT, KPD_ENT}};
//=========================
// Objects
//=========================

//=========================
// Function prototypes
//=========================
// draw должен стать указателем на функцию отображения одного из экранов. Либо обработка нужного экрана будет идти внутри него
void draw();
// так же и keypadEvent - в зависимости от экрана должен менятся контекст
void keypadEvent(KeypadEvent key);

void setPosition(int dir, int b_isRel);
void jog(int dir);
volatile char key;

hw_timer_t *FluidStatusTimer = NULL;

void ARDUINO_ISR_ATTR GetCNCStatus()
{
    if (abs((int)(millis() - CNC.lastResponceTime)) >= 240)
    {
        CNC.requestFluidStatus();
    }
}

U8G2_ST7920_128X64_1_HW_SPI u8g2(U8G2_R0, 12);
Keypad myKeypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);
FluidSender_UI_Class UI;

void setup()
{

    Serial.begin(115200);                      // Debug serial through USB-C
    Serial1.begin(115200, SERIAL_8N1, 39, 40); // Main serial to communicate with Fence //Serial1.begin(BAUD, SERIAL_8N1, RX_GPIO, TX_GPIO);

    u8g2.setBusClock(400000);
    u8g2.begin();
    u8g2.enableUTF8Print();
    u8g2.setFont(u8g2_font_haxrcorp4089_t_cyrillic);

    myKeypad.addEventListener(keypadEvent);

    FluidStatusTimer = timerBegin(0, 8000, 1);
    if (FluidStatusTimer != NULL)
    {
        timerAttachInterrupt(FluidStatusTimer, GetCNCStatus, 1);
        timerAlarmWrite(FluidStatusTimer, 2500, 1);
        timerAlarmEnable(FluidStatusTimer);
        _DebugPrintLn("Timer initiated");
    }
    else
        _DebugPrintLn("Timer initiation failed");

    _DebugPrintLn("Sender has started!");

    UI.begin(&u8g2, &myKeypad);
    FluidNC_Parser.reset();
    CNC.begin();

    UI.context = &mainContext;
}

void loop()
{
    WiFi_Check();
    FluidNC_Parser.update();
    UI.draw();
    myKeypad.getKey();
}

// #define B_COORD_REL 1 // удалить
// #define B_COORD_ABS 0 // удалить

void keypadEvent(KeypadEvent key)
{
    switch (myKeypad.getState())
    {
    case PRESSED:
    {
        _DebugPrint("Key pressed: ");
        _DebugPrintLn(key); // Дебаг для вывода нажатой кнопки

        // if (CNC.x.getHomingStatus() == HOME_OK)
        // {
        if (key >= '0' && key <= '9')
        {
            if ((CNC.inputPos.length() <= 4))
            {
                if (key == '0' && !CNC.inputPos.length())
                    return;

                CNC.inputPos += key;
            }

            if (CNC.inputPos.toInt() > 1500)
                CNC.inputPos.clear();

            _DebugPrintLn(CNC.inputPos);
        }

        if (key == KPD_AST)
            CNC.inputPos += '.';

        switch (key)
        {
        case KPD_F1:

            break;
        case KPD_F2:
            CNC.x.setHomingStatus(HOME_OK);
            break;
        case KPD_ESC:
            CNC.inputPos.clear();

            break;
        case KPD_ENT: // OK
            // setPosition(0, B_COORD_ABS);
            CNC.x.moveTo(CNC.inputPos.toFloat());
            break;
        case KPD_LT: //<-
            // setPosition(1, B_COORD_REL);
            CNC.x.moveBy(CNC.inputPos.toFloat(), 0);
            break;
        case KPD_RT: //->
            // setPosition(0, B_COORD_REL);
            CNC.x.moveBy(CNC.inputPos.toFloat(), 1);
            break;
        default:
            break;
        }
        // homingStatus == Ok-----------------------------------------
        // }
    }

    case RELEASED:
        break;

    case HOLD:
        switch (key)
        {
        // case KPD_F1:
        // {
        //     CNC.inputPos = ""; // Set position to home
        //     // setPosition(0, B_COORD_ABS);
        //     CNC.requestFluidStatus();
        //     break;
        // }
        //<-
        case KPD_LT:
        { //<-
            // jog(1);
            _DebugPrintLn("Jog left");

            break;
        }
        //->
        case KPD_RT:
        {
            // jog(0);
            _DebugPrintLn("Jog rigth");

            break;
        }
        case KPD_ESC:
        {
            CNC.reset();
            break;
        }
        case KPD_F1:
        {
            CNC.x.setHomingStatus(HOME_OK);
            _DebugPrintLn("Home");
            _CNC_Print("$H\n");
            // CNCPrint("$$\n"); // Запросить настройки
            break;
        }
        case KPD_AST:
        {
            FluidNC_Updater(&UI);
            break;
        }
        }
    }
}
