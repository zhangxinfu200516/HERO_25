#include "Steering_wheel_group.h"

void Class_steering_motor::Streeing_Motor_TIM_PID_PeriodElapsedCallback()
{
    switch (DJI_Motor_Control_Method)
    {
    case (DJI_Motor_Control_Method_OPENLOOP):
    {
        //默认开环扭矩控制
        Out = Target_Torque / Torque_Max * Output_Max;
    }
    break;
    case (DJI_Motor_Control_Method_TORQUE):
    {
        //默认闭环扭矩控制
        Out = Target_Torque / Torque_Max * Output_Max;
    }
    break;
    case (DJI_Motor_Control_Method_OMEGA):
    {
        PID_Omega.Set_Target(Target_Omega_Radian);
        PID_Omega.Set_Now(steer_actual_omega);
        PID_Omega.TIM_Adjust_PeriodElapsedCallback();

        Out = PID_Omega.Get_Out();
    }
    break;
    case (DJI_Motor_Control_Method_ANGLE):
    {
        PID_Angle.Set_Target(Target_Radian);
        PID_Angle.Set_Now(steer_actual_angle);
        PID_Angle.TIM_Adjust_PeriodElapsedCallback();

        Target_Omega_Radian = PID_Angle.Get_Out();

        PID_Omega.Set_Target(Target_Omega_Radian);
        PID_Omega.Set_Now(steer_actual_omega);
        PID_Omega.TIM_Adjust_PeriodElapsedCallback();

        Out = PID_Omega.Get_Out();
    }
    break;
    default:
    {
        Out = 0.0f;
    }
    break;
    }
    Output();
}
void Class_Steering_Wheel_Group::TIM_Control_PeriodElapsedCallback()
{
    if(Now_Control_Falg == Last_Control_Flag)
        Control_Method = Steering_Wheel_Group_Control_Disable;
    else
        Control_Method = Steering_Wheel_Group_Control_Enable;
    Last_Control_Flag = Now_Control_Falg;
}
void Class_Steering_Wheel_Group::Init(Enum_Steering_Wheel_Group_Control_Method __Control_Method)
{
    Control_Method = __Control_Method;
    //布瑞特编码器
    briter_encoder.Init(&hcan2,A_ENCODER_ID,get_total_angle,Encoder_Baud_1M);
    //转向电机
    steering_motor.Init(&hcan1,DJI_Motor_ID_0x201,DJI_Motor_Control_Method_ANGLE,10.0f);//减速比待修正
    steering_motor.PID_Angle.Init(0.0f,0.0f,0.0f,0.0f,2000.0f,16500.0f,0.0f,0.0f,0.0f);
    steering_motor.PID_Omega.Init(0.0f,0.0f,0.0f,0.0f,2000.0f,16500.0f,0.0f,0.0f,0.0f);
    //动力电机
    motion_motor.Init(&hcan1,DJI_Motor_ID_0x202,DJI_Motor_Control_Method_OMEGA,3591/187.0f);//减速比待修正
    motion_motor.PID_Omega.Init(0.0f,0.0f,0.0f,0.0f,2000.0f,16500.0f,0.0f,0.0f,0.0f);
}

void Class_Steering_Wheel_Group::Output()
{
    switch (Control_Method)
    {
    case Steering_Wheel_Group_Control_Disable:
        /* code */
        //设置相应状态失能
        //布瑞特编码器
        briter_encoder.Set_BriterEncoder_ControlMethod(control_disable);
        //动力电机
        motion_motor.Set_DJI_Motor_Control_Method(DJI_Motor_Control_Method_OMEGA);
        motion_motor.PID_Omega.Set_Integral_Error(0.0f);
        motion_motor.Set_Out(0.0f);
        //转向电机
        steering_motor.Set_DJI_Motor_Control_Method(DJI_Motor_Control_Method_OMEGA);
        steering_motor.PID_Omega.Set_Integral_Error(0.0f);
        steering_motor.Set_Out(0.0f);
        break;
    case Steering_Wheel_Group_Control_Enable://切换状态条件：接收到云台CAN通信发送的数据
        /* code */
        briter_encoder.Set_BriterEncoder_ControlMethod(get_total_angle);
        motion_motor.Set_DJI_Motor_Control_Method(DJI_Motor_Control_Method_ANGLE);
        steering_motor.Set_DJI_Motor_Control_Method(DJI_Motor_Control_Method_OMEGA);
        //布瑞特编码器和转向电机<---舵轮角度 :弧度
        /*code*/
        float tmp_steer_target_angle = set_angle;
        float tmp_steer_actual_angle = (float)(briter_encoder.Get_Single_Turn_Encoder())/(float)briter_encoder.Get_Lsbs_Per_Part_Round()*PI*2.0f;
        float tmp_steer_actual_omega = steering_motor.Get_Now_Omega_Angle();
        steering_motor.Set_Steer_Actual_Angle(tmp_steer_actual_angle);
        steering_motor.Set_Steer_Actual_Omega(tmp_steer_actual_omega);

        float theta = tmp_steer_target_angle - steering_motor.Get_Steer_Actual_Angle();
        if(fabsf(theta)>PI/2.0f)
            tmp_steer_target_angle += PI;
        while(tmp_steer_target_angle>2.0f*PI)
            tmp_steer_target_angle -= 2.0f*PI;
        while(theta<-2.0f*PI)
            tmp_steer_target_angle += 2.0f*PI;
        
        motion_motor.Set_Target_Radian(tmp_steer_target_angle);
        //动力电机<---舵轮角速度 ：弧度每秒
        /*code*/
        motion_motor.Set_Target_Omega_Radian(cosf(theta)*steering_motor.Get_Steer_Actual_Omega());
        break;
    }
}
void Class_Steering_Wheel_Group::TIM_Calculate_PeriodElapsedCallback()
{
    TIM_Control_PeriodElapsedCallback();
    Output();

    briter_encoder.TIM_Briter_Encoder_PeriodElapsedCallback();
    motion_motor.TIM_PID_PeriodElapsedCallback();
    steering_motor.Streeing_Motor_TIM_PID_PeriodElapsedCallback();
}