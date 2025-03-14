#ifndef __FluidNC_CNC_H__
#define __FluidNC_CNC_H__

class FluidNC_CNC_Axis_Class
{
  private:

  float realPos, localPos, min, max;
  float localOffset;

  public:
 //Переписать на uint8_t или задать свой тип, чтобы обеспечить проверку на ошибки
  void setRealPos(); //Устанавливает действительное положение с проверкой на минимальное и максимальное
  void setLocalPos(); //Нужно ли, или должно вычисляться автоматически?
  void setMin(); //Принимает значение из настроек при инициализации или можно выставить самому?
  void setMax(); //Принимает значение из настроек при инициализации или можно выставить самому?

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
  
};

#endif