#ifndef __FluidNC_Ctrl_H__
#define __FluidNC_Ctrl_H__

////////////////////FluidNC_Ctrl class

enum Fluid_Status
{
  FENCE_STATUS_STARTED,
  FENCE_STATUS_IDLE,
  FENCE_STATUS_JOG,
  FENCE_STATUS_HOMING,
  FENCE_STATUS_ALARM,
  FENCE_STATUS_ERROR
};

enum Fluid_Home_Status
{
  FENCE_NOT_HOMED,
  FENCE_HOMED_OK,
  FENCE_HOMING_FAIL
};

enum Fluid_Bool{
  FLUID_FALSE,
  FLUID_TRUE
};

class FluidNC_Class
{
  private:
//boolean
  unsigned int b_Use_Status_report = FLUID_TRUE;  // $10
  unsigned int b_Use_Soft_Limits = FLUID_TRUE;    // $20
  unsigned int b_Use_Hard_Limits = FLUID_FALSE;   // $21
  unsigned int b_Use_Homing = FLUID_TRUE;         // $22

//Max Speednle Speed, RPM
  unsigned int Max_Spindle_Speed = 0; // $30

// Steps/mm
  float X_Steps = 80.0f; // $100
  float Y_Steps = 80.0f; // $101
  float Z_Steps = 80.0f; // $102

 // Max feeding rate, mm/min
  float X_Max_Rate = 1000.0f; // $110
  float Y_Max_Rate = 1000.0f; // $111
  float Z_Max_Rate = 1000.0f; // $112


// Max acceleration, mm/sec^2
  float X_Acceleration = 25.0f; // $120
  float Y_Acceleration = 25.0f; // $121
  float Z_Acceleration = 25.0f; // $122

// Max travel , mm
  float X_Max_Travel = 1000.0f; // $130
  float Y_Max_Travel = 1000.0f; // $131
  float Z_Max_Travel = 1000.0f; // $132

  public:
    String inputPos;

    float xPos, yPos, zPos, currPos, prevPos, desiredPos, minPos, maxPos;
    int b_isHomed;
    unsigned int status;
    
    void setXPos(int pos);
    void setYPos(int pos);
    void setZPos(int pos);
    void setStatus(int status);
    

    int getStatus();

    void changeStatus(int status)
    {
      this->b_isHomed = status;
      if(status == FENCE_HOMED_OK)
      this->status = FENCE_STATUS_IDLE;
      else
      switch(status)
      {
        case FENCE_STATUS_IDLE: this->status = FENCE_STATUS_IDLE;
        break;
        case FENCE_STATUS_JOG: this->status = FENCE_STATUS_JOG;
        break;
        case FENCE_STATUS_HOMING: this->status = FENCE_STATUS_HOMING;
        break;        
        case FENCE_STATUS_ALARM: this->status = FENCE_STATUS_ALARM;
        break;
        default: this->status = FENCE_STATUS_ERROR;
        break;
      }
    }

    FluidNC_Class()
    {
      status = FENCE_STATUS_STARTED;
      xPos = 0.0f;
      yPos = 0.0f;
      zPos = 0.0f;

      minPos = 2.0f;
      maxPos = 1500.0f;

      // memset(inputPos,'\0', sizeof(inputPos));
      inputPos = "";

      currPos = minPos;

      prevPos = currPos;
      desiredPos = currPos;

      b_isHomed = FENCE_NOT_HOMED;
    }

}Fence;

#endif 