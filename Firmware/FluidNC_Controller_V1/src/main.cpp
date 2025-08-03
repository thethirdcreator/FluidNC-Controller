#include <Arduino.h>
#include <U8g2lib.h> // Драйвер дисплея
#include <AutoOTA.h> // Обновление по воздуху
#include <WiFi.h>
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

// hw_timer_t *BaseTimer = NULL;
// volatile uint64_t isrCounter = 0;

// void ARDUINO_ISR_ATTR onTimer()
// {

//   isrCounter++;
// }

#define fontName u8g2_font_7x13_mf
#define fontX 7
#define fontY 16
#define offsetX 0
#define offsetY 3
#define U8_Width 128
#define U8_Height 64

U8G2_ST7920_128X64_1_HW_SPI u8g2(U8G2_R0, 12);  
FluidSender_UI_t UI;

void setup()
{

    Serial.begin(115200);                      // Debug serial through USB-C
    Serial1.begin(115200, SERIAL_8N1, 39, 40); // Main serial to communicate with Fence //Serial1.begin(BAUD, SERIAL_8N1, RX_GPIO, TX_GPIO);

    u8g2.setBusClock(400000);
    u8g2.begin();
    u8g2.enableUTF8Print();
    u8g2.setFont(u8g2_font_haxrcorp4089_t_cyrillic);

    // keypad.addEventListener(keypadEvent);

    _DebugPrintLn("Sender has started!");

    UIbeginCustom(&UI);
    FluidNC_Parser.reset();
    CNC.reset();
}

void loop()
{
    WiFi_Check();
    FluidNC_Parser.update();
    // UI.draw();
    //   key = keypad.getKey();
}

// void draw()
// {
//   u8g2.firstPage();
//   do
//   {

//     u8g2.setCursor(128 - u8g2.getStrWidth("V:1.X"), 64);
//     u8g2.print("V:");
//     u8g2.print(ota.version());

//     u8g2.setCursor(0, 64);
//     u8g2.print("IP:");
//     u8g2.print(WiFi.localIP());

//     if (Fence.b_isHomed)
//     {
//       // Вывод текущего положения
//       u8g2.setCursor(40, 24);
//       u8g2.print(Fence.xPos);

//       if (Fence.inputPos.length()) // Отображать только, когда ввод не нулевой
//       {
//         // Вывод нового положения
//         u8g2.setCursor(64 - (Fence.inputPos.length() * 4) - u8g2.getStrWidth("->"), 24 + 5 + 16);
//         u8g2.print("->");
//         u8g2.print(Fence.inputPos);
//         u8g2.print("<-");
//       }
//     }
//     else
//     {
//       u8g2.setCursor(0, 24);
//       u8g2.print("нужен хоуминг");
//       u8g2.setCursor(0, 34);
//       u8g2.print("зажми F1");
//       u8g2.setCursor(0, 54);
//       u8g2.print("<- движение ->");
//     }

//   } while (u8g2.nextPage());
// }

// #define B_COORD_REL 1 // удалить
// #define B_COORD_ABS 0 // удалить

// void keypadEvent(KeypadEvent key)
// {
//   switch (keypad.getState())
//   {
//   case PRESSED:
//   {
//     _DebugPrint("Key pressed: ");
//     _DebugPrintLn(key); // Дебаг для вывода нажатой кнопки

//     if (Fence.b_isHomed)
//     {
//       // isHomed == Ok-----------------------------------------
//       if (key >= '0' && key <= '9')
//       {
//         if ((Fence.inputPos.length() <= 4))
//         {
//           if (key == '0' && !Fence.inputPos.length())
//             return;

//           Fence.inputPos += key;
//         }

//         if (Fence.inputPos.toInt() > 1500)
//           Fence.inputPos = "";

//         Serial.println(Fence.inputPos);
//       }

//       if (key == KPD_AST)
//         Fence.inputPos += '.';

//       switch (key)
//       {
//       case KPD_F1:

//         break;
//       case KPD_F2:
//         Fence.changeStatus(FENCE_HOMED_OK);
//         break;
//       case KPD_ESC:

//         break;
//       case KPD_ENT: // OK
//         // setPosition(0, B_COORD_ABS);
//         break;
//       case KPD_DN: //<-
//         // setPosition(1, B_COORD_REL);
//         break;
//       case KPD_UP: //->
//         // setPosition(0, B_COORD_REL);
//         break;
//       default:
//         break;
//       }
//       // isHomed == Ok-----------------------------------------
//     }
//   }

//   case RELEASED:
//     break;

//   case HOLD:
//     switch (key)
//     {
//     case KPD_F1:
//     {
//       Fence.inputPos = "0"; // Set position to home
//       // setPosition(0, B_COORD_ABS);
//       break;
//     }
//     case KPD_LT:
//     { //<-
//       // jog(1);
//       Serial.println("Jog left");

//       break;
//     }
//     case KPD_RT:
//     { //->
//       // jog(0);
//       Serial.println("Jog rigth");

//       break;
//     }
//     case KPD_ESC:
//     {
//       Fence.inputPos = "";
//       break;
//     }
//     case KPD_F2:
//     {
//       Fence.changeStatus(FENCE_HOMED_OK);
//       Serial.println("Home");
//       Serial1.print("$H");
//       Serial1.print('\n');
//       // CNCPrint("$$\n"); // Запросить настройки
//       break;
//     }
//     case KPD_AST:
//     {
//       FluidNC_Updater();
//       break;
//     }
//     }
//   }
// }
