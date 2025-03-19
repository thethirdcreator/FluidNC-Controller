#ifndef __FENCE_PARSER_H__
#define __FENCE_PARSER_H__

#include <string.h>
#include "FluidNC_Ctrl.h"

#define parserDebugMode

#ifdef parserDebugMode
#define parserDebugPrintln(X)     \
  Serial.print("Parser debug: "); \
  Serial.println(X)
#else
#define parserDebugPrintln(X) ;
#endif

#define numChars 100

char UART_RX_Data[numChars];

boolean newData = false;

// Func prototypes
void fenceReceiveUart();
void parseData();
void parseMsg();
void parseCmd();
void flushUart();
void parseStatus();
//

void fenceReceiveUart()
{
  static byte ndx = 0;
  char endMarker = '\n';
  char rc;

  while (Serial1.available() > 0 && newData == false)
  {
    rc = Serial1.read();

    if (rc != endMarker)
    {
      UART_RX_Data[ndx] = rc;
      ndx++;
      if (ndx >= numChars)
      {
        ndx = numChars - 1;
      }
    }
    else
    {
      UART_RX_Data[ndx] = '\0'; // terminate the string
      ndx = 0;
      Serial.print("Transitting: ");
      Serial.println(UART_RX_Data);
      parseData();
    }
  }
}

void parseData()
{

  switch (UART_RX_Data[0])
  {
  case '<':
    parseStatus();
    break;
  case '$':
    parseCmd();
    break;
  case '[':
    parseMsg();
    break;
  default:
  {
    Serial.print("Line rejected: ");
    parserDebugPrintln(UART_RX_Data);
    flushUart();
    return;
  }
  }
}

void parseMsg()
{
  Serial.println("This is a message");
}
void parseCmd()
{
  Serial.println("Parsing setting:");
  switch (UART_RX_Data[1])
  {
  }
}
void parseStatus()
{
  Serial.println("This is a status");
  char status[10];
  memset(status, '\0', sizeof(status));

  switch (UART_RX_Data[1])
  {
  case 'I':
  { // Idle
    Serial.println("Idle");
    Fence.changeStatus(FENCE_STATUS_IDLE);
    break;
  }
  case 'H':
  { // Home
    Serial.println("Home");
    Fence.changeStatus(FENCE_STATUS_HOMING);
    break;
  }
  case 'A':
  { // Alarm
    Serial.println("Alarm");
    Fence.changeStatus(FENCE_STATUS_ALARM);
    break;
  }
  case 'J': // Jog
  case 'M':
  { // Move
    Serial.println("Jog");
    Fence.changeStatus(FENCE_STATUS_JOG);
    break;
  }
  }

  char *pch = strtok(UART_RX_Data, " <>|:,"); // Status
  pch = strtok(NULL, " <>|:,");               // MPos
  Fence.xPos = atof(strtok(NULL, " <>|:,"));  // X
  Fence.yPos = atof(strtok(NULL, " <>|:,"));  // Y
  Fence.zPos = atof(strtok(NULL, " <>|:,"));  // Z
}

void flushUart()
{
  memset(UART_RX_Data, '\0', sizeof(UART_RX_Data));
}

#endif