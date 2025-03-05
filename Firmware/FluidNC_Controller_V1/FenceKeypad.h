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


const uint8_t ROWS = 5;
const uint8_t COLS = 4;
char keys[ROWS][COLS] = {
  { KPD_F1, KPD_F2, KPD_HASH, KPD_AST},
  { '1',    '2',    '3',      KPD_UP},
  { '4',    '5',    '6',      KPD_DN},
  { '7',    '8',    '9',      KPD_ESC},
  { KPD_LT, '0',    KPD_RT,   KPD_ENT}
};
uint8_t colPins[COLS] = { 4, 3, 2, 1 };
uint8_t rowPins[ROWS] = { 13, 10, 8, 6, 5};

void get_input();


Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

#endif