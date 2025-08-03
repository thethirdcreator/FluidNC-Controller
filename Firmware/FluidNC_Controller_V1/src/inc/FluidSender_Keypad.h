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

#define ROWS  5
#define COLS  4

// extern Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);
extern Keypad myKeypad;

#endif