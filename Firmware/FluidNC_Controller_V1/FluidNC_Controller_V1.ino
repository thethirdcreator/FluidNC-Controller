#include <LiquidCrystal_I2C.h>
#include <U8g2lib.h>
//#include <ESP32TimerInterrupt.h>
#include "FluidNC_Ctrl.h"
#include "FenceParser.h"
#include "FenceKeypad.h"
#include "FluidNC_Cmd.h"

#define LCD_I2C_ADDR 0x27
#define LCD_COLUMNS 16
#define LCD_LINES 2

#define B_COORD_REL 1
#define B_COORD_ABS 0

#define MIN_POS 2

//LCD DEFINES
#define LCD_HOME_INF (0, 0)
#define LCD_POS_INF (0, 7)
#define LCD_COORD_INF (1, 0)

LiquidCrystal_I2C lcd(LCD_I2C_ADDR, LCD_COLUMNS, LCD_LINES);
U8G2_ST7920_128X64_1_HW_SPI u8g2(U8G2_R0, 12);
// FluidNC_Class Fence(); // Объявление происходит в файле описания класса. Костыль.

void keypadEvent(KeypadEvent key);

hw_timer_t *BaseTimer = NULL;
volatile uint64_t isrCounter = 0;
volatile uint64_t displayTime = 0;
volatile uint64_t keypadTime = 0;
unsigned long long newCall;
static volatile unsigned long long lastCall;
volatile char key;

portMUX_TYPE displayMux = portMUX_INITIALIZER_UNLOCKED;

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

  // Serial.print("Xpos = ");
  // Serial.println(Fence.xPos);
  // Serial.print("Ypos = ");
  // Serial.println(Fence.yPos);
  // Serial.print("Zpos = ");
  // Serial.println(Fence.zPos);
  // Serial.print("MINpos = ");
  // Serial.println(Fence.minPos);
  // Serial.print("MAXpos = ");
  // Serial.println(Fence.maxPos);
}

void loop() {

  // displayTime = millis();
  // if ((millis() - lastCall) > 100)
  draw(); //12864 display only
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
    u8g2.drawGlyph(5, 20, 0x2603); // снеговик
    u8g2.setCursor(0, 20);
    u8g2.drawGlyph(5, 60, 0x2615); // Кофе

    u8g2.setCursor(50, 50);
    u8g2.print(isrCounter);
  } while (u8g2.nextPage());
}

void lcd_update() {
  unsigned long long newCall = millis();
  static unsigned long long lastCall;

  if ((newCall - lastCall) < 200)
    return;

  lastCall = millis();

  lcd.clear();
  //Отрисовываем статус
  lcd.setCursor(0, 0);
  switch (Fence.status) {
    case FENCE_STATUS_STARTED:
      lcd.print("Need homing");
      break;
    case FENCE_STATUS_IDLE:
      lcd.print("Idle");
      break;
    case FENCE_STATUS_JOG:
      lcd.print("Moving");
      break;
    case FENCE_STATUS_HOMING:
      lcd.print("Homing");
      break;
    case FENCE_STATUS_ALARM:
      lcd.print("ALARM");
      break;
    default:
      lcd.print("ERROR");
      break;
  }

  //Отрисовываем текущее положение
  lcd.setCursor(0, 1);
  if (Fence.b_isHomed)
    lcd.print(Fence.xPos);
  else {
    lcd.print("POS=?");
  }
  //Отрисовываем вводимое расстояние
  lcd.setCursor(7, 1);
  lcd.print(Fence.inputPos);
}



void get_input() {

  // char key = keypad.getKey();

  // if (key >= '0' && key <= '9') {
  //   if ((Fence.inputPos.length() <= 4))
  //     Fence.inputPos += key;

  //   if (Fence.inputPos.toInt() > 1500)
  //     Fence.inputPos = "";

  //   Serial.println(Fence.inputPos);
  // }


  // // else
  // switch (key)
  // {
  //   case 'A': //F1
  //     Fence.changeStatus(FENCE_HOMED_OK);
  //     break;
  //   case 'B': { //F2

  //       Serial.println("Home");
  //       Serial1.print("$H");
  //       Serial1.print('\n');

  //       break;
  //     }
  //   case 'C': //,
  //     // Fence.b_IsAbsCoord = Fence.b_IsAbsCoord? 0:1;
  //     break;
  //   case 'D': //OK
  //     setPosition( 0, B_COORD_ABS);
  //     break;
  //   case '#': //<-
  //     setPosition( 0, B_COORD_REL);
  //     break;
  //   case '*': //->
  //     setPosition( 1, B_COORD_REL);
  //     break;

  // }
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
//G53 - abs
//G91 - rel
//G21 - mm
// $  - gcode cmd

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


void keypadEvent(KeypadEvent key) 
{
  switch (keypad.getState()) 
  {
    case PRESSED:
    {
        Serial.println(key);  // Дебаг для вывода нажатой кнопки
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
              Fence.inputPos = "0"; //Set position to home
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
