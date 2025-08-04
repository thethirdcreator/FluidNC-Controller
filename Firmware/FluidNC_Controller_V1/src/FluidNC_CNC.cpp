#include "inc/FluidNC_CNC.h"

FluidNC_CNC_Class CNC;

void FluidNC_CNC_Class::reset()
{
    _CNC_Write(_FLUID_CMD_CANCEL);
    _CNC_Print("$X\n");
    _CNC_Print("$T\n");
    this->x.setPos(0.0f);
    this->y.setPos(0.0f);
    this->z.setPos(0.0f);
    this->inputPos.clear();
    this->status = FLUID_IDLE;
    // сброс внутренних переменных для осей
    // this->x.
}
// -----------------------------
// Axis
// -----------------------------
void FluidNC_CNC_Axis_Class::setPos(float pos)
{
    this->realPos = pos;
}

float FluidNC_CNC_Axis_Class::getLocalPos()
{
    return this->localPos;
}

float FluidNC_CNC_Axis_Class::getRealPos()
{
    return this->realPos;
}

FluidHomingStatus FluidNC_CNC_Axis_Class::getHomingStatus()
{
    return this->homingStatus;
}

void FluidNC_CNC_Axis_Class::setHomingStatus(FluidHomingStatus status)
{
    this->homingStatus = status;
}
// -----------------------------
// CNC
// -----------------------------
void FluidNC_CNC_Class::changeStatus(FluidStatus status)
{
    this->status = status;
}
void FluidNC_CNC_Class::requestFluidStatus()
{
    _CNC_Print("$?\n");
    // _CNC_Print("$T");
}

FluidStatus FluidNC_CNC_Class::getStatus()
{
    return status;
}

// -----------------------------
// Other
// -----------------------------
String statusToText(FluidStatus status)
{
    switch (status)
    {
    case FLUID_ALARM:
        return "Alarm";
        break;
    case FLUID_IDLE:
        return "Idle";
        break;
    case FLUID_MOVING:
        return "Moving";
        break;
    case FLUID_JOG:
        return "Jog";
        break;
    case FLUID_HOMING:
        return "Homing";
        break;
    defualt:
        return "Undefined";
        break;
    }
    return "Error";
}
