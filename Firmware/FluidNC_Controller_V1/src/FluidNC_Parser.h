#ifndef __FluidNC_PARSER_H__
#define __FluidNC_PARSER_H__

#include <Arduino.h>

#define Serial1 _Port

typedef enum ParserStatus : int8_t {
  PARSER_ERR_UNDEFINED = -127,
  PARSER_REJECTED = -1,
  PARSER_READY = 0,
  PARSER_BUSY,
  PARSER_MESSAGE,
  PARSER_COMMAND,
  PARSER_BUSY
};

typedef class FluidNC_Parser_Class
{
  private:
  uint8_t ndx;
  ParserStatus status;

  public:

  


};



#endif