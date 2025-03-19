#include "FluidNC_Parser.h"

FluidNC_Parser_Class::parser_status_t FluidNC_Parser_Class::begin()
{
  return PARSER_IDLE;
}

FluidNC_Parser_Class::parser_status_t FluidNC_Parser_Class::poll()
{
  while (_Port.available() > 0 && this->status == PARSER_IDLE)
  {
    this->rxChar = _Port.read();

    if (rxChar != _EndMarker)
    {
      this->rxBuffer[this->rxNdx] = this->rxChar;
      this->ndx++;
      if (this->rxNdx >= _MaxRxChars)
      {
        this->rxNdx = _MaxRxChars - 1;
      }
    }
    else
    {
      this->rxBuffer[this->rxNdx] = '\0'; // terminate the string
      this->rxNdx = 0;

      _parserDebugPrint("Transit string: ");
      _parserDebugPrintln((char *)this->rxBuffer);

      this->status = PARSER_RECEIVED;

      this->parse();
    }
  }

  return PARSER_IDLE;
}

FluidNC_Parser_Class::parser_status_t FluidNC_Parser_Class::parse()
{
  

  return PARSER_IDLE;
}
