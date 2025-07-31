#include "inc/FluidNC_Parser.hpp"

FluidNC_Parser_Class FluidNC_Parser;

void FluidNC_Parser_Class::update()
{
    switch (state)
    {
    case ST_NONE:
        this->receiveChar();
        break;
    case ST_PREPASS:
        this->prePass();
        break;
    case ST_PARSE_CMD:
        this->parseCmd();
        break;
    case ST_PARSE_DATA:
        this->parseData();
        break;
    case ST_PARSE_STATUS:
        this->parseStatus();
        break;
    default:
        _DebugPrintLn("Parser default state occured");
        this->reset();
        break;
    }
    // prePass();
    // _DebugPrintLn("OK");
}

void FluidNC_Parser_Class::reset()
{
    memset(this->RX_BUFF, 0, sizeof(this->RX_BUFF));
    this->buff_ndx = 0;
    this->state = ST_NONE;
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
            this->state = ST_PREPASS;
            // prePass();
            this->update();
        }
        else
        {
            this->RX_BUFF[this->buff_ndx++] = c;
            // очистка буфера, сброс счетчика буфера, сброс состояния

            if (this->buff_ndx >= UART_RX_BUFF_SIZE)
                this->reset();
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
        this->state = ST_PARSE_STATUS;
        break;
    case '$':
        this->state = ST_PARSE_CMD;
        break;
    case '[':
        this->state = ST_PARSE_MSG;
        break;
    default:
    {
        _DebugPrint("Line rejected: ");
        _DebugPrintLn(this->RX_BUFF);
        this->reset();
        return;
    }
    }
    this->update();
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

void FluidNC_Parser_Class::parseData()
{
    _DebugPrintLn("This is a data");
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

    pch = strtok(RX_BUFF, " <>|:,");            // Status
    pch = strtok(NULL, " <>|:,");               // MPos
    CNC.x.setPos(atof(strtok(NULL, " <>|:,"))); // X
    CNC.y.setPos(atof(strtok(NULL, " <>|:,"))); // Y
    CNC.z.setPos(atof(strtok(NULL, " <>|:,"))); // Z
}