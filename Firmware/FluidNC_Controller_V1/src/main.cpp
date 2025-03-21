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

#include "WiFiList.h"
#include "FluidNC_Ctrl.h"
#include "FenceParser.h"
#include "FenceKeypad.h"
#include "FluidNC_Cmd.h"

#include "FluidNC_Updater.h" // Ахуенный костыль. Переделать срочно

#include "FluidNC_CNC.h"

//=========================
// Firmware version
//=========================


//=========================
// Defines
//=========================
#define DebugPrint(X) Serial.print(X)
#define DebugPrintln(X) Serial.println(X)
#define CNCPrint(X) Serial1.print(X)
#define CNCPrintln(X) Serial1.println(X)

//=========================
// Objects
//=========================


//=========================
// Function prototypes
//=========================
void keypadEvent(KeypadEvent key);
void draw();
void setPosition(int dir, int b_isRel);
void jog(int dir);
//=========================
// Function prototypes end
//=========================

hw_timer_t *BaseTimer = NULL;
volatile uint64_t isrCounter = 0;

void ARDUINO_ISR_ATTR onTimer()
{

  isrCounter++;
}

// дебажная хуйня

char _ssid[16] = {"SSID here      "};
char _password[16] = {"Password here  "};

// char *constMEM hexDigit MEMMODE = "1234567890ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz%*)?@#$~-_ ";
// char *constMEM hexNr[] MEMMODE = {hexDigit};

// result resetAllNahui()
// {
//   ESP.restart();
//   return proceed;
// }

// int chooseTest = 1;
// CHOOSE(chooseTest, chooseMenu, "Choose", doNothing, noEvent, noStyle,
//        VALUE("First", 1, doNothing, noEvent),
//        VALUE("Second", 2, doNothing, noEvent),
//        VALUE("Third", 3, doNothing, noEvent),
//        VALUE("EbalRot", 4, doNothing, noEvent),
//        VALUE("HuiV04ko", 5, doNothing, noEvent),
//        VALUE("Suka", 6, doNothing, noEvent),
//        VALUE("Reset", 7, resetAllNahui, anyEvent));

// MENU(wifiMenu, "WiFi Settings", doNothing, noEvent, wrapStyle,
//      EDIT("SSID: ", _ssid, hexNr, doNothing, noEvent, noStyle),
//      EDIT("Password: ", _password, hexNr, doNothing, noEvent, noStyle),
//      SUBMENU(chooseMenu),
//      EXIT("<Back"));

// MENU(mainMenu, "Main menu", doNothing, noEvent, wrapStyle,
//      OP("Ebalaika", doNothing, noEvent),
//      OP("Ebalaika2", doNothing, noEvent),
//      SUBMENU(wifiMenu),
//      EXIT("<Back"));

#define MAX_DEPTH 3

#define fontName u8g2_font_7x13_mf
#define fontX 7
#define fontY 16
#define offsetX 0
#define offsetY 3
#define U8_Width 128
#define U8_Height 64
// #define USE_HWI2C

// const colorDef<uint8_t> colors[6] MEMMODE = {
//     {{0, 0}, {0, 1, 1}}, // bgColor
//     {{1, 1}, {1, 0, 0}}, // fgColor
//     {{1, 1}, {1, 0, 0}}, // valColor
//     {{1, 1}, {1, 0, 0}}, // unitColor
//     {{0, 1}, {0, 0, 1}}, // cursorColor
//     {{1, 1}, {1, 0, 0}}, // titleColor
// };

// MENU_OUTPUTS(out, MAX_DEPTH,
//              U8G2_OUT(u8g2, colors, fontX, fontY, offsetX, offsetY, {0, 0, U8_Width / fontX, U8_Height / fontY}),
//              SERIAL_OUT(Serial));

// MENU_OUTPUTS(out, MAX_DEPTH,
//              SERIAL_OUT(Serial),
//              NONE);

// keypadIn myKpad(keypad);
// serialIn mySerialIn(Serial);
// MENU_INPUTS(myInputs, &myKpad, &mySerialIn);
// MENU_INPUTS(myInputs, &mySerialIn, NONE);

// NAVROOT(nav, mainMenu, MAX_DEPTH, myInputs, out);

// дебажная хуйня end

void setup()
{

  Serial.begin(115200);                      // Debug serial through USB-C
  Serial1.begin(115200, SERIAL_8N1, 39, 40); // Main serial to communicate with Fence //Serial1.begin(BAUD, SERIAL_8N1, RX_GPIO, TX_GPIO);

  u8g2.setBusClock(400000);
  u8g2.begin();
  u8g2.enableUTF8Print();

  // keypad.addEventListener(keypadEvent);

  // BaseTimer = timerBegin(1000, , true);
  // timerAttachInterrupt(BaseTimer, &onTimer);
  // timerAlarmEnable(BaseTimer, 100, true, 0);

  Serial.println("Started...");
  delay(10000);
  Serial.print("Trying to connect to ");
  Serial.println(ssid);

  Serial.println(WiFi.begin(ssid, password));

  wl_status_t WiFiStatus;
  Serial.println("Connecting to WiFi..");

  do
  {
    delay(1000);
    WiFiStatus = WiFi.status();
    Serial.print(".");
    Serial.println(WiFiStatus);
  } while (!((WiFiStatus == WL_CONNECTED) || (WiFiStatus == WL_CONNECT_FAILED) || (WiFiStatus == WL_DISCONNECTED)));

  if (WiFiStatus == WL_CONNECTED)
  {
    Serial.println("");
    Serial.println("WiFi connected");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
  }
  else
  {
    Serial.println("");
    Serial.println("Connection failed");
  }
 
}

void loop()
{

  draw(); // 12864 display only
  static char key = keypad.getKey();
  // keypad.getKey();
  // nav.poll();
  fenceReceiveUart();
  Serial1.print("$J=");
  delay(100);
}

void draw()
{
  u8g2.firstPage();
  do
  {
    u8g2.setFont(u8g2_font_unifont_t_cyrillic); // Set Russian font
    u8g2.setCursor(0, 32);


    u8g2.setFont(u8g2_font_spleen5x8_mr);
    u8g2.setCursor(0, 10);
    u8g2.print("V: ");
    u8g2.print(ota.version());

    u8g2.setCursor(0, 64);
    u8g2.print("IP: ");
    u8g2.print(WiFi.localIP());
  } while (u8g2.nextPage());
}

void setPosition(int dir, int b_isRel)
{

  if (Fence.inputPos.length() == 0)
    return;

  Serial1.print("$J=");
  Serial1.print("G21");

  if (b_isRel)
    Serial1.print("G91");
  else
    Serial1.print("G53");

  if (dir)
    Serial1.print("X-");
  else
    Serial1.print("X");

  Serial1.print(Fence.inputPos);

  Serial1.print("F5000");
  Serial1.write(0x0A);

  Fence.inputPos = "";
}

void jog(int dir)
{
  Serial1.print("$J=");
  Serial1.print("G21");
  Serial1.print("G91");

  if (dir)
    Serial1.print("X-");
  else
    Serial1.print("X");

  Serial1.print("10");

  Serial1.print("F1000");
  Serial1.write(0x0A);
}

#define B_COORD_ABS 0 // удалить
#define B_COORD_REL 0 // удалить
void keypadEvent(KeypadEvent key)
{
  switch (keypad.getState())
  {
  case PRESSED:
  {
    Serial.println(key); // Дебаг для вывода нажатой кнопки
    if (Fence.b_isHomed)
    {
      // isHomed == Ok-----------------------------------------
      if (key >= '0' && key <= '9')
      {
        if ((Fence.inputPos.length() <= 4))
          Fence.inputPos += key;

        if (Fence.inputPos.toInt() > 1500)
          Fence.inputPos = "";

        Serial.println(Fence.inputPos);
      }

      if (key == KPD_AST)
        Fence.inputPos += '.';

      switch (key)
      {
      case KPD_F1:
        Fence.inputPos = "0"; // Set position to home
        setPosition(0, B_COORD_ABS);
        break;
      case KPD_F2:
        Fence.changeStatus(FENCE_HOMED_OK);
        break;
      case KPD_ESC:

        break;
      case KPD_ENT: // OK
        setPosition(0, B_COORD_ABS);
        break;
      case KPD_DN: //<-
        setPosition(1, B_COORD_REL);
        break;
      case KPD_UP: //->
        setPosition(0, B_COORD_REL);
        break;
      default:
        break;
      }
      // isHomed == Ok-----------------------------------------
    }
  }

  case RELEASED:
    break;

  case HOLD:
    switch (key)
    {
    case KPD_F1:
    {
      Fence.changeStatus(FENCE_HOMED_OK);
      Serial.println("Home");
      Serial1.print("$H");
      Serial1.print('\n');
      break;
    }
    case KPD_DN:
    { //<-
      jog(1);
      Serial.println("Jog left");

      break;
    }
    case KPD_UP:
    { //->
      jog(0);
      Serial.println("Jog rigth");
      break;
    }
    case KPD_ESC:
    {
      Fence.inputPos = "";
      break;
    }
    case KPD_F2:
    {
      CNCPrint("$$\n");
      break;
    }
    case KPD_AST:
    {
      void FluidNC_Updater();
      break;
    }

    }
  }
}
