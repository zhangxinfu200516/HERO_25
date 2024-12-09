#include "ita_chariot.h"

Class_Wheel_Group wheel_group;
uint16_t Flag;
void Class_Wheel_Group::Init(Enum_Wheel_Group_Control_Method __Control_Method)
{
    steering_wheel_group.Init(Steering_Wheel_Group_Control_Enable);
}

#define PI 3.14159265358979323846
#define MAX_WHEEL_OMEGA 94.26f//3508电机在C620电调下额定转速为：469rpm, MAX_WHEEL_OMEGA = 469*2π/60*（3591/187）(减速比)
void Class_Wheel_Group::CAN_WheelGroup_Rx_Classis_Callback()
{
    Flag++;
    steering_wheel_group.Set_Now_Control_Flag(Flag);

    uint16_t tmp_angle,tmp_omega;
    float tmp_set_angle,tmp_set_omega;
    memcpy(&tmp_angle,&CAN2_Manage_Object.Rx_Buffer.Data[0],2);
    memcpy(&tmp_omega,&CAN2_Manage_Object.Rx_Buffer.Data[2],2);
    tmp_set_angle = Math_Int_To_Float(tmp_angle,0,0xFFFF,-PI,PI);
    tmp_set_omega = Math_Int_To_Float(tmp_omega,0,0xFFFF,-MAX_WHEEL_OMEGA,MAX_WHEEL_OMEGA);
    steering_wheel_group.Set_Set_Angle(tmp_set_angle);
    steering_wheel_group.Set_Set_Omega(tmp_set_omega);

}

void Class_Wheel_Group::TIM_Calculate_PeriodElapsedCallback()
{
    steering_wheel_group.TIM_Calculate_PeriodElapsedCallback();
}