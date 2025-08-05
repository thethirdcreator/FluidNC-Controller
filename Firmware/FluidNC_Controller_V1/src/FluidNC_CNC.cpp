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

void FluidNC_CNC_Axis_Class::moveTo(float pos)
{
    _CNC_Print("$J=");

    // if (b_isRel)
    // // G91 - Incremental distance mode.
    //     Serial1.print("G91");
    // else
    // // G53 Use Machine Coordinates
    //     Serial1.print("G53");

    _CNC_Print("G53");
    // G1 Motion at Feed Rate
    // _CNC_Print("G1 ");
    // Ось
    _CNC_Print("X");
    _CNC_Print(CNC.inputPos);

    _CNC_Print(" F");
    _CNC_Print(speed);
    _CNC_Print("\n");

    CNC.inputPos.clear();

}

void FluidNC_CNC_Axis_Class::moveBy(float pos, uint8_t dir)
{
    _CNC_Print("$J=");

    // if (b_isRel)
    // // G91 - Incremental distance mode.
    //     Serial1.print("G91");
    // else
    // // G53 Use Machine Coordinates
    //     Serial1.print("G53");

    _CNC_Print("G91");
    // G1 Motion at Feed Rate
    // _CNC_Print("G1 ");
    // Ось
    if(dir)
    _CNC_Print("X");
    else
    _CNC_Print("X-");

    _CNC_Print(CNC.inputPos);

    _CNC_Print(" F");
    _CNC_Print(speed);
    _CNC_Print("\n");

    CNC.inputPos.clear();

}
// -----------------------------
// CNC
// -----------------------------
void FluidNC_CNC_Class::begin()
{
    reset();
    // G21 - Use millimeters for length and speed values
    _CNC_Print("G21");
    lastResponceTime = millis();

    this->x.speed = 2300;
    this->y.speed = 2300;
    this->z.speed = 2300;
}

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
