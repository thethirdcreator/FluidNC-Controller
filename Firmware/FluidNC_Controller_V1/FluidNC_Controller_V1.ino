#include <U8g2lib.h>
#include <AutoOTA.h>
#include <WiFi.h>
#include "WiFiList.h"
#include "FluidNC_Ctrl.h"
#include "FenceParser.h"
#include "FenceKeypad.h"
#include "FluidNC_Cmd.h"

#define FluidNC_Controller_Ver "1.02"

#define DebugPrint(X) Serial.print(X)
#define DebugPrintln(X) Serial.println(X) 
#define CNCPrint(X) Serial1.print(X)
#define CNCPrintln(X) Serial1.println(X)

U8G2_ST7920_128X64_1_HW_SPI u8g2(U8G2_R0, 12);

void keypadEvent(KeypadEvent key);

hw_timer_t *BaseTimer = NULL;
volatile uint64_t isrCounter = 0;
// volatile uint64_t displayTime = 0;
// volatile uint64_t keypadTime = 0;
volatile char key;

void ARDUINO_ISR_ATTR onTimer() {
  // key = keypad.getKey();

  // newCall = millis();
  // if ((newCall - lastCall) > 100){
  // // portENTER_CRITICAL_ISR(&displayMux);
  // draw();
  // // portEXIT_CRITICAL_ISR(&displayMux);
  // }

  // lastCall = millis();

  isrCounter++;
}


void setup() {

  Serial.begin(115200);                       //Debug serial through USB-C
  Serial1.begin(115200, SERIAL_8N1, 39, 40);  // Main serial to communicate with Fence //Serial1.begin(BAUD, SERIAL_8N1, RX_GPIO, TX_GPIO);

  u8g2.begin();
  u8g2.setBusClock(400000);

  keypad.addEventListener(keypadEvent);

  BaseTimer = timerBegin(1000);
  timerAttachInterrupt(BaseTimer, &onTimer);
  timerAlarm(BaseTimer, 100, true, 0);

  Serial.println("Started...");

  WiFi.begin(ssid, password);
  String wifiStatus;
  String localIp;
  while (wifiStatus != WL_CONNECTED || wifiStatus !=WL_CONNECT_FAILED) {
  delay(500);
  Serial.println("Connecting to WiFi..");
  wifiStatus = WiFi.status();
}
if(wifiStatus == WL_CONNECTED){
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  localIp = WiFi.localIP();
  Serial.println(localIp);
}
else
{ 
  Serial.println("");
  Serial.println("Connection failed");
  localIp = "Not Connected"
}

  AutoOTA ota(FluidNC_Controller_Ver, "thethirdcreator/FluidNC-Controller/tree/master/Firmware/FluidNC_Controller_V1");
if (ota.checkUpdate()) {
    ota.updateNow();
}


}

void loop() {

  // displayTime = millis();
  // if ((millis() - lastCall) > 100)
  draw();  //12864 display only
  // lastCall = millis();
  // Serial.print("Draw time: ");
  // Serial.println((millis() - displayTime));

  // keypadTime = millis();
  key = keypad.getKey();
  // Serial.print("Keypad time: ");
  // Serial.println((millis() - keypadTime));

  fenceReceiveUart();
}

void draw() {
  u8g2.firstPage();
  do {
    u8g2.setFont(u8g2_font_ncenB14_tr);  // Set Russian font
    u8g2.setCursor(0, 32);
    u8g2.print("Hello world!");

    u8g2.setFont(u8g2_font_unifont_t_symbols);
    u8g2.setCursor(0, 0);
    u8g2.drawGlyph(5, 20, 0x2603);  // снеговик
    u8g2.setCursor(0, 20);
    u8g2.drawGlyph(5, 60, 0x2615);  // Кофе

    u8g2.setCursor(20, 10);
    u8g2.print(WiFi.localIP());
    u8g2.setCursor(50, 50);
    u8g2.print(isrCounter);
    u8g2.setCursor(20, 60);
    u8g2.print(FluidNC_Controller_Ver);
  } while (u8g2.nextPage());
}

void setPosition(int dir, int b_isRel) {

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

void jog(int dir) {
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

#define B_COORD_ABS 0  //удалить
#define B_COORD_REL 0  //удалить
void keypadEvent(KeypadEvent key) {
  switch (keypad.getState()) {
    case PRESSED:
    {
        Serial.println(key);  // Дебаг для вывода нажатой кнопки
        if (Fence.b_isHomed) 
        {
          // isHomed == Ok-----------------------------------------
          if (key >= '0' && key <= '9') {
            if ((Fence.inputPos.length() <= 4))
              Fence.inputPos += key;

            if (Fence.inputPos.toInt() > 1500)
              Fence.inputPos = "";

            Serial.println(Fence.inputPos);
          }

          if (key == KPD_AST)
            Fence.inputPos += '.';

          switch (key) {
            case KPD_F1:
              Fence.inputPos = "0";  //Set position to home
              setPosition(0, B_COORD_ABS);
              break;
            case KPD_F2:
              Fence.changeStatus(FENCE_HOMED_OK);
              break;
            case KPD_ESC:

              break;
            case KPD_ENT:  //OK
              setPosition(0, B_COORD_ABS);
              break;
            case KPD_DN:  //<-
              setPosition(1, B_COORD_REL);
              break;
            case KPD_UP:  //->
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
      switch (key) {
        case KPD_F1:
          {
            Fence.changeStatus(FENCE_HOMED_OK);
            Serial.println("Home");
            Serial1.print("$H");
            Serial1.print('\n');
            break;
          }
        case KPD_DN:
          {  //<-
            jog(1);
            Serial.println("Jog left");

            break;
          }
        case KPD_UP:
          {  //->
            jog(0);
            Serial.println("Jog rigth");
            break;
          }
        case KPD_ESC:
          {
            Fence.inputPos = "";
            break;
          }
      }
  }
}
