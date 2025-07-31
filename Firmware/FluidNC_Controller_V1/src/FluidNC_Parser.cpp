#include "inc/FluidNC_Parser.hpp"

void FluidNC_Parser_Class::reset()
{
    memset(this->RX_BUFF, 0, sizeof(this->RX_BUFF));
    this->buff_ndx = 0;
    this->state = ST_NONE;

    this->update = &FluidNC_Parser_Class::receiveChar;
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
    switch (this->RX_BUFF[0])
    {
    case '<':
        this->update = &FluidNC_Parser_Class::parseStatus;
        break;
    case '$':
        this->update = &FluidNC_Parser_Class::parseCmd;
        break;
    case '[':
        this->update = &FluidNC_Parser_Class::parseMsg;
        break;
    default:
    {
        _DebugPrint("Line rejected: ");
        _DebugPrintLn(this->RX_BUFF);
        // this->reset();
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
    switch (this->RX_BUFF[1])
    {
    }
}

void FluidNC_Parser_Class::parseStatus()
{
    _DebugPrintLn("This is a status");
    char status[10];
    char *pch;
    memset(status, '\0', sizeof(status));

    switch (this->RX_BUFF[1])
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

    pch = strtok(this->RX_BUFF, " <>|:,");      // Status
    pch = strtok(NULL, " <>|:,");              // MPos
    CNC.x.setPos(atof(strtok(NULL, " <>|:,"))); // X
    CNC.y.setPos(atof(strtok(NULL, " <>|:,"))); // Y
    CNC.z.setPos(atof(strtok(NULL, " <>|:,"))); // Z
}