#ifndef __FluidNC_PARSER_H__
#define __FluidNC_PARSER_H__

#include <Arduino.h>



#define _Port Serial1
#define _EndMarker '\n'
#define _MaxRxChars 255

#define _parserDebugMode

#ifdef _parserDebugMode
#define _parserDebugPrintln(X) Serial.println(X)
#else
#define parserDebugPrintln(X) ;
#endif

#ifdef _parserDebugMode
#define _parserDebugPrint(X) Serial.print(X)
#else
#define parserDebugPrint(X) ;
#endif

class FluidNC_Parser_Class
{

public:
typedef enum : int8_t
{
  PARSER_ERR_UNDEFINED = -127,
  PARSER_REJECTED = -1,
  PARSER_IDLE = 0,
  PARSER_RECEIVED,
  PARSER_BUSY,
  PARSER_MESSAGE,
  PARSER_COMMAND,
  PARSER_STATUS
}parser_status_t;

private:
  uint8_t rxNdx, ndx;
  uint8_t rxChar;
  uint8_t rxBuffer[_MaxRxChars];

  parser_status_t status;

public:
  parser_status_t begin(); // do nothing
  parser_status_t poll();
  parser_status_t parse();

  FluidNC_Parser_Class()
  {
    ndx = 0;
    rxNdx = 0;
    rxChar = '\0';
    memset(rxBuffer, '\0', sizeof(rxBuffer));
  }

};

#endif