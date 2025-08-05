#ifndef __FluidNC_CNC_HPP__
#define __FluidNC_CNC_HPP__

#include <stdint.h>
#include <Arduino.h>

#define _CNC_Port Serial1
#define _CNC_Write(Data) _CNC_Port.write(Data)
#define _CNC_Print(Data) _CNC_Port.print(Data)
#define _CNC_PrintLn(Data) _CNC_Port.println(Data)

#define _FLUID_CMD_CANCEL 0x18

enum FluidBool : uint8_t
{
    FluidFalse = 0,
    FluidTrue
};

enum FluidStatus : uint8_t
{
    FLUID_ALARM = 0,
    FLUID_IDLE,
    FLUID_MOVING,
    FLUID_JOG,
    FLUID_HOMING,
    FLUID_ERR_UNDEFINED = 255
};

enum FluidHomingStatus : int8_t
{
    HOME_ERR = -1,
    HOME_NONE,
    HOME_OK
};

class FluidNC_CNC_Axis_Class
{
private:
    //=========================
    // This variables are acquired from FluidNC settings
    //=========================
    float maxTravel, acceleration, steps, maxRate;

    //=========================
    // This variables are acquired by processing settings and through user input
    //=========================
    int32_t maxPos, minPos;
    float realPos, localPos;
    float localOffset;
    uint32_t feedRate;
    FluidHomingStatus homingStatus;
    
    FluidStatus setRealPos();  // Устанавливает действительное положение с проверкой на минимальное и максимальное
    FluidStatus setLocalPos(); // Нужно ли, или должно вычисляться автоматически?
    FluidStatus setMin();      // Принимает значение из настроек при инициализации или можно выставить самому?
    FluidStatus setMax();      // Принимает значение из настроек при инициализации или можно выставить самому?
    
    public:
    uint32_t speed;
    FluidStatus axisDoHoming();
    void setHomingStatus(FluidHomingStatus);
    FluidHomingStatus getHomingStatus();

    float getRealPos();  // Отправляет действительное положение
    float getLocalPos(); // Отправляет действительное положение с поправкой на сдвиг (зубцов диска)5
    float getMin();      // Отдает float значение
    float getMax();      // Отдает float значение

    void setPos(float pos);
    void moveTo(float pos);
    void moveBy(float pos, uint8_t dir);
};

class FluidNC_CNC_Class
{
private:
    String fluidNCVersion; // Заменить на char[]

    uint8_t bIsInited;
    uint8_t bIsFailed;
    FluidStatus status;

public:
    FluidNC_CNC_Axis_Class x, y, z;
    unsigned long lastResponceTime;

    FluidStatus getStatus();
    String inputPos;

    void begin();
    void reset();
    void changeStatus(FluidStatus status);
    void requestFluidStatus();
};

String statusToText(FluidStatus);

extern FluidNC_CNC_Class CNC;

#endif