#include "inc/FluidNC_CNC.h"

FluidNC_CNC_Class CNC;

void FluidNC_CNC_Class::reset()
{
    _CNC_Write(_FLUID_CMD_RESET);
    this->x.setPos(0.0f);
    this->y.setPos(0.0f);
    this->z.setPos(0.0f);
    this->inputPos.clear();
    this->status = FLUID_IDLE;
    // сброс внутренних переменных для осей
    // this->x.
}

void FluidNC_CNC_Axis_Class::setPos(float pos)
{
    this->realPos = pos;
}

float FluidNC_CNC_Axis_Class::getLocalPos()
{
    return this->localPos;
}

FluidHomingStatus FluidNC_CNC_Axis_Class::getHomingStatus()
{
    return this->homingStatus;
}

void FluidNC_CNC_Axis_Class::setHomingStatus(FluidHomingStatus status)
{
    this->homingStatus = status;
}

void FluidNC_CNC_Class::changeStatus(FluidStatus status)
{
    this->status = status;
}

FluidStatus FluidNC_CNC_Class::getStatus()
{
    return status;
}
