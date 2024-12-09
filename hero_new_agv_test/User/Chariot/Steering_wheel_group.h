#ifndef _STEERING_WHEEL_GROUP_H_
#define _STEERING_WHEEL_GROUP_H_
#ifdef  __cplusplus
#include "dvc_djimotor.h"
#include "dvc_briter_encode.h"

#define A_ZERO_POS 0

enum Enum_Steering_Wheel_Group_Control_Method
{
    Steering_Wheel_Group_Control_Disable,
    Steering_Wheel_Group_Control_Enable,
};
class Class_steering_motor : public  Class_DJI_Motor_C620
{
public:
    inline float Get_Steer_Actual_Angle();
    inline float Get_Steer_Actual_Omega();
    inline float Set_Steer_Actual_Angle(float __angle);
    inline float Set_Steer_Actual_Omega(float __omega);
    void Streeing_Motor_TIM_PID_PeriodElapsedCallback();
protected:
    float steer_actual_angle;//舵机实际转向角 单位：弧度
    float steer_actual_omega;//舵机实际转向角速度 单位： 弧度每秒

};
float Class_steering_motor::Get_Steer_Actual_Angle()
{
    return (steer_actual_angle);
}
float Class_steering_motor::Get_Steer_Actual_Omega()
{
    return (steer_actual_omega);
}
float Class_steering_motor::Set_Steer_Actual_Angle(float __angle)
{
    steer_actual_angle = __angle;
}
float Class_steering_motor::Set_Steer_Actual_Omega(float __omega)
{
    steer_actual_omega = __omega;
}
class Class_Steering_Wheel_Group
{
public:
    Class_DJI_Motor_C620 motion_motor;
    Class_steering_motor steering_motor;
    Class_Briter_Encoder briter_encoder;
    inline float Get_Set_Omega();
    inline float Get_Set_Angle();
    inline float Set_Set_Angle(float __Set_Angle);
    inline float Set_Set_Omega(float __Set_Omega);
    inline uint16_t Set_Now_Control_Flag(uint16_t flag);
    void Init(Enum_Steering_Wheel_Group_Control_Method __Control_Method = Steering_Wheel_Group_Control_Disable);
    void TIM_Calculate_PeriodElapsedCallback();
    void TIM_Control_PeriodElapsedCallback();
protected:
    Enum_Steering_Wheel_Group_Control_Method Control_Method;
    float set_omega;//舵轮组中动力电机的转速 单位：rad/s
    float set_angle;//舵轮组中舵机的转角 单位：rad 
    uint16_t Now_Control_Falg;
    uint16_t Last_Control_Flag;
    //内部函数
    void Output();
};
float Class_Steering_Wheel_Group::Get_Set_Omega()
{
    return (set_omega);
}
float Class_Steering_Wheel_Group::Get_Set_Angle()
{
    return (set_angle);
}
float Class_Steering_Wheel_Group::Set_Set_Angle(float __Set_Angle)
{
    set_angle = __Set_Angle;
}
float Class_Steering_Wheel_Group::Set_Set_Omega(float __Set_Omega)
{
    set_omega = __Set_Omega;
}
uint16_t Class_Steering_Wheel_Group::Set_Now_Control_Flag(uint16_t flag)
{
    Now_Control_Falg = flag;
}
#endif
#endif
