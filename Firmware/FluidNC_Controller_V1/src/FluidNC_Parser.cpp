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
      this->rxNdx++;
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
  switch (this->status)
  {
    // PARSER_RECEIVED start
  case PARSER_RECEIVED:
  {
    switch (this->rxBuffer[this->ndx])
    {
    case '<':
      this->status = PARSER_STATUS;
      break;
    case '$':
      this->status = PARSER_COMMAND;
      break;
    case '[':
      this->status = PARSER_MESSAGE;
      break;
    default:
    {
      _parserDebugPrint("Parsing error. Line rejected: ");
      _parserDebugPrintln((char *)this->rxBuffer);

      this->flush(); // перенести в конец всех итераций парсинга
      this->status = PARSER_REJECTED;
    }
      ndx++;
      this->parse();
    }
    break;
  } // PARSER_RECEIVED end

    // PARSER_STATUS start
  case PARSER_STATUS:
  {

    break;
  } // PARSER_STATUS end

    // PARSER_COMMAND start
  case PARSER_COMMAND:
  {

    break;
  } // PARSER_COMMAND end

  // PARSER_MESSAGE start
  case PARSER_MESSAGE:
  {

    break;
  } // PARSER_MESSAGE end

  // PARSER_REJECTED start
  case PARSER_REJECTED:
  {
    this->flush();
    break;
  } // PARSER_REJECTED end
  default:
  {
    _parserDebugPrint("Parsing error critical!\n Trying to show buffer:");
    _parserDebugPrintln((char *)this->rxBuffer);
    this->flush();
    return PARSER_ERR_UNDEFINED;
  }
  }

  return PARSER_IDLE;
}

FluidNC_Parser_Class::parser_status_t FluidNC_Parser_Class::flush()
{
  memset(this->rxBuffer, '\0', sizeof(this->rxBuffer));
  this->rxNdx = 0;
  this->ndx = 0;
  this->status = PARSER_IDLE;

  return PARSER_IDLE;
}
