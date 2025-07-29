#ifndef __FluidNC_CNC_HPP__
#define __FluidNC_CNC_HPP__

#include <stdint.h>
#include "FluidNC_Parser.h"

enum FluidBool : uint8_t
{
  FluidFalse = 0,
  FluidTrue
};

enum FluidStatus : int8_t
{
  FLUID_ERR_UNDEFINED = -127,
  FLUID_ERR = 0,
  FLUID_IDLE,
  FLUID_OK
};

enum FluidHomingStatus : int8_t
{
  HOME_ERR = -1,
  HOME_NONE,
  HOME_OK
};

enum FluidAxisStatus : int8_t
{
  AXIS_ERR = -1,
  AXIS_IDLE,
  AXIS_MOVING
};

class FluidNC_CNC_Axis_Class
{
private:
  //=========================
  // This variables are acquired from FluidNC settings
  //=========================
  float maxTravel, acceleration, steps, maxRate;
  String fluidNCVersion;

  //=========================
  // This variables are acquired by processing settings and through user input
  //=========================
  int32_t maxPos, minPos;
  float realPos, localPos;
  float localOffset;
  uint32_t feedRate;
  FluidHomingStatus isHomed;

  FluidStatus setRealPos();  // Устанавливает действительное положение с проверкой на минимальное и максимальное
  FluidStatus setLocalPos(); // Нужно ли, или должно вычисляться автоматически?
  FluidStatus setMin();      // Принимает значение из настроек при инициализации или можно выставить самому?
  FluidStatus setMax();      // Принимает значение из настроек при инициализации или можно выставить самому?
  FluidHomingStatus setHomingStatus(FluidHomingStatus);

public:
  FluidStatus axisDoHoming();
  FluidHomingStatus getHomingStatus();

  float getRealPos();  // Отправляет действительное положение
  float getLocalPos(); // Отправляет действительное положение с поправкой на сдвиг (зубцов диска)5
  float getMin();      // Отдает float значение
  float getMax();      // Отдает float значение
};

class FluidNC_CNC_Class
{
private:
public:
  FluidNC_CNC_Axis_Class x, y, z;

  void begin();
  void reset();
};

#endif