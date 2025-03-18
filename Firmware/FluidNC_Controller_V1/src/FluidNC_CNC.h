#ifndef __FluidNC_CNC_H__
#define __FluidNC_CNC_H__

#include <stdint.h>

typedef enum FluidBool : uint8_t {
  FluidFalse = 0,
  FluidTrue
};

typedef enum FluidStatus : int8_t {
  FLUID_ERR_UNDEFINED = -127,
  FLUID_IDLE = 0,
  FLUID_OK
};

typedef enum FluidHomingStatus : int8_t {
  HOME_ERR = -1,
  HOME_NONE,
  HOME_OK
};

typedef enum FluidAxisStatus : int8_t {
  AXIS_ERR = -1,
  AXIS_IDLE,
  AXIS_
};

class FluidNC_CNC_Axis_Class
{
  private:

  float maxTravel, minTravel;
  float realPos, localPos;
  float localOffset;
  uint32_t feedRate;
  FluidHomingStatus isHomed;

  FluidStatus setRealPos(); //Устанавливает действительное положение с проверкой на минимальное и максимальное
  FluidStatus setLocalPos(); //Нужно ли, или должно вычисляться автоматически?
  FluidStatus setMin(); //Принимает значение из настроек при инициализации или можно выставить самому?
  FluidStatus setMax(); //Принимает значение из настроек при инициализации или можно выставить самому?
  FluidHomingStatus setHomingStatus(FluidHomingStatus);

  public:

  FluidStatus axisDoHomeing();
  FluidHomingStatus getHomeingStatus();
  
  float getRealPos(); // Отправляет действительное положение
  float getLocalPos(); // Отправляет действительное положение с поправкой на сдвиг (зубцов диска)
  float getMin(); //Отдает float значение
  float getMax(); //Отдает float значение
  

};

class FluidNC_CNC_Class
{
  private:
  FluidNC_CNC_Axis_Class x,y,z;

  public:

  void begin();
  
};

#endif