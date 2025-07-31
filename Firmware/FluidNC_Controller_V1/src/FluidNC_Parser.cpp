#include "inc/FluidNC_Parser.hpp"

FluidNC_Parser_Class FluidNC_Parser;

void FluidNC_Parser_Class::update()
{
    
}

void FluidNC_Parser_Class::reset()
{
    memset(RX_BUFF, 0, sizeof(RX_BUFF));
    buff_ndx = 0;
    state = ST_NONE;
}

void FluidNC_Parser_Class::receiveChar()
{
    char c;

    while (CNC_PORT.available() > 0)
    {
        c = CNC_PORT.read();

        if (c == END_MARKER)
        {
            this->RX_BUFF[this->buff_ndx] = '\0'; // terminate the string
            // Заменить на обработчик парсинга
            // this->update = parseData;
            state = ST_PREPASS;
            prePass();
        }
        else
        {
            this->RX_BUFF[this->buff_ndx++] = c;
            // очистка буфера, сброс счетчика буфера, сброс состояния
            // this->reset();
            reset();
            // Можно делать и так:
            // Перезаписываем последний символ, пока не получим конец сообщения
            // Но в любом из случаев надо добавить сброс по времени ожидания

            // if (this->buff_ndx >= UART_RX_BUFF_SIZE)
            // {
            //   this->buff_ndx = UART_RX_BUFF_SIZE - 1;
            // }
        }
    }
}

// определяет тип сообщения
void FluidNC_Parser_Class::prePass()
{
    switch (RX_BUFF[0])
    {
    case '<':
        state = ST_PARSE_STATUS;
        break;
    case '$':
        state = ST_PARSE_CMD;
        break;
    case '[':
        state = ST_PARSE_MSG;
        break;
    default:
    {
        _DebugPrint("Line rejected: ");
        _DebugPrintLn(RX_BUFF);
        reset();
        return;
    }
    }
}

void FluidNC_Parser_Class::parseMsg()
{
    _DebugPrintLn("This is a message");
}

void FluidNC_Parser_Class::parseCmd()
{
    _DebugPrintLn("Parsing setting:");
    switch (RX_BUFF[1])
    {
    }
}

void FluidNC_Parser_Class::parseStatus()
{
    _DebugPrintLn("This is a status");
    char status[10];
    char *pch;
    memset(status, '\0', sizeof(status));

    switch (RX_BUFF[1])
    {
    case 'I':
    { // Idle
        _DebugPrintLn("Idle");
        CNC.changeStatus(FLUID_IDLE);
        break;
    }
    case 'H':
    { // Home
        _DebugPrintLn("Home");
        CNC.changeStatus(FLUID_HOMING);
        break;
    }
    case 'A':
    { // Alarm
        _DebugPrintLn("Alarm");
        CNC.changeStatus(FLUID_ALARM);
        break;
    }
    case 'J': // Jog
    case 'M':
    { // Move
        _DebugPrintLn("Jog");
        CNC.changeStatus(FLUID_JOG);
        break;
    }
    }

    pch = strtok(RX_BUFF, " <>|:,");      // Status
    pch = strtok(NULL, " <>|:,");               // MPos
    CNC.x.setPos(atof(strtok(NULL, " <>|:,"))); // X
    CNC.y.setPos(atof(strtok(NULL, " <>|:,"))); // Y
    CNC.z.setPos(atof(strtok(NULL, " <>|:,"))); // Z
}