#include "inc/FluidNC_CNC.hpp"

FluidNC_CNC_Class CNC;

void FluidNC_CNC_Class::reset()
{
    _CNC_Write(_FLUID_CMD_RESET);
    this->x.setPos(0.0f);
    this->y.setPos(0.0f);
    this->z.setPos(0.0f);
    // сброс внутренних переменных для осей
    // this->x.
}

void FluidNC_CNC_Axis_Class::setPos(float pos)
{
    this->realPos = pos;
}

void FluidNC_CNC_Class::changeStatus(FluidStatus status)
{
    this->status = status;
}

FluidStatus FluidNC_CNC_Class::getStatus()
{
    return status;
}