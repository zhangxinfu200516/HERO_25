#ifndef _ITA_CHARIOT_H_
#define _ITA_CHARIOT_H_
#ifdef __cplusplus
#include "struct_typedef.h"
#include "drv_math.h"
#include "Steering_wheel_group.h"
enum Enum_Wheel_Group_Control_Method
{
    wheel_group_control_disable,
    wheel_group_control_enable,
};
class Class_Wheel_Group
{
public:
    Class_Steering_Wheel_Group steering_wheel_group;
    //成员函数
    void Init(Enum_Wheel_Group_Control_Method __Control_Method = wheel_group_control_enable);
    void CAN_WheelGroup_Rx_Classis_Callback();
    void TIM_Calculate_PeriodElapsedCallback();
protected:
    //内部函数
    
};
extern Class_Wheel_Group wheel_group;
#endif
#endif
