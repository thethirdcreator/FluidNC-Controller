#ifndef __FENCE_KEYPAD_H__
#define __FENCE_KEYPAD_H__

#include <Keypad.h>

#define KPD_F1 'Z'
#define KPD_F2 'X'
#define KPD_HASH 'Y'
#define KPD_AST 'W'
#define KPD_UP 'A'
#define KPD_DN 'B'
#define KPD_ESC 'C'
#define KPD_ENT 'D'
#define KPD_LT '*'
#define KPD_RT '#'
#define KPD_1 '1'
#define KPD_2 '2'
#define KPD_3 '3'
#define KPD_4 '4'
#define KPD_5 '5'
#define KPD_6 '6'
#define KPD_7 '7'
#define KPD_8 '8'
#define KPD_9 '9'
#define KPD_0 '0'

const uint8_t ROWS = 5;
const uint8_t COLS = 4;

char keys[ROWS][COLS] = {
  { KPD_F1, KPD_F2, KPD_HASH, KPD_AST},
  { KPD_1,  KPD_2,  KPD_3,    KPD_UP},
  { KPD_4,  KPD_5,  KPD_6,    KPD_DN},
  { KPD_7,  KPD_8,  KPD_9,    KPD_ESC},
  { KPD_LT, KPD_0,  KPD_RT,   KPD_ENT}
};

uint8_t colPins[COLS] = { 4, 3, 2, 1 };
uint8_t rowPins[ROWS] = { 13, 10, 8, 6, 5};

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

#endif