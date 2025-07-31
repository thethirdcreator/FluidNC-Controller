#ifndef __FLUID_NC_PARSER_HPP__
#define __FLUID_NC_PARSER_HPP__

#include <string.h>
#include <StringUtils.h>
#include <Arduino.h>
#include "inc/FluidNC_CNC.hpp"
#include "inc/Debug.hpp"

#define UART_RX_BUFF_SIZE 100
#define END_MARKER '\n'
#define CNC_PORT Serial1

enum FluidNC_Parser_State_t
{
    ST_NONE = 0,
    ST_PARSE_STATUS,
    ST_PARSE_DATA,
    ST_PARSE_MSG,
    ST_PARSE_CMD
};

class FluidNC_Parser_Class
{
private:
    FluidNC_Parser_State_t state;
    char RX_BUFF[UART_RX_BUFF_SIZE];
    ptrdiff_t buff_ndx;
    void flushBuff();
    void receiveChar();
    void parseStatus();
    void parseData();
    void parseMsg();
    void parseCmd();
    void prePass();

public:
    void reset();
    // https://isocpp.org/wiki/faq/pointers-to-members
    void (FluidNC_Parser_Class::*update)(); // Handler указывающий на текущую операцию

    FluidNC_Parser_Class()
    {
        update = &FluidNC_Parser_Class::prePass;
        reset();
    }

} FluidNC_Parser;

#endif