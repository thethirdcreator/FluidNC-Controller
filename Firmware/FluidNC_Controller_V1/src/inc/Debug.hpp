#ifndef __DEBUG_HPP__
#define __DEBUG_HPP__

#include <Arduino.h>

#define _bEnableDebug
#define _bEnableLog

#ifdef _bEnableDebug
#define _DebugPrint(X) Serial.print(X)
#define _DebugPrintLn(X) Serial.println(X)
#else
#define _DebugPrint(X)
#define _DebugPrintLn(X)
#endif

#ifdef _bEnableLog
#define _LogPrint(X) Serial.print(X)
#define _LogPrintLn(X) Serial.println(X)
#else
#define _LogPrint(X)
#define _LogPrintLn(X)
#endif


#endif