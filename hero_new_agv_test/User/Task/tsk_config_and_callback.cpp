#include "tsk_config_and_callback.h"
#include "drv_can.h"
#include "ita_chariot.h"
#include "drv_tim.h"

bool start_flag=0;
uint32_t init_finished =0;

#define ENCODER_A
void System_Device_CAN1_Callback(Struct_CAN_Rx_Buffer *CAN_RxMessage)
{
    switch (CAN_RxMessage->Header.StdId)
    {
    case 0x201:
        wheel_group.steering_wheel_group.steering_motor.CAN_RxCpltCallback(CAN_RxMessage->Data);
        break;
    case 0x202:
        wheel_group.steering_wheel_group.motion_motor.CAN_RxCpltCallback(CAN_RxMessage->Data);
        break;
    #ifdef ENCODER_A
    case 0X0A:
        wheel_group.steering_wheel_group.briter_encoder.CAN_RxCpltCallback(CAN_RxMessage->Data);
        break;
    #endif
        #ifdef ENCODER_B
    case 0X0B:
        steering_wheel_group.briter_encoder.CAN_RxCpltCallback(CAN_RxMessage->Data);
        break;
    #endif
        #ifdef ENCODER_C
    case 0X0C:
        steering_wheel_group.briter_encoder.CAN_RxCpltCallback(CAN_RxMessage->Data);
        break;
    #endif
        #ifdef ENCODER_D
    case 0X0D:
        steering_wheel_group.briter_encoder.CAN_RxCpltCallback(CAN_RxMessage->Data);
        break;
    #endif
    default:
        break;
    }
}

void System_Device_CAN2_Callback(Struct_CAN_Rx_Buffer *CAN_RxMessage)
{
    switch (CAN_RxMessage->Header.StdId)
    {
    #ifdef ENCODER_A
    case 0x101:
        wheel_group.CAN_WheelGroup_Rx_Classis_Callback();
        break;
    #endif
    #ifdef ENCODER_B
    case 0x102:
        wheel_group.CAN_WheelGroup_Rx_Classis_Callback();
        break;
    #endif
    #ifdef ENCODER_C
    case 0x103:
        wheel_group.CAN_WheelGroup_Rx_Classis_Callback();
        break;
    #endif
    #ifdef ENCODER_D
    case 0x104:
        wheel_group.CAN_WheelGroup_Rx_Classis_Callback();
        break;
    #endif
    default:
        break;
    }
}

void Task1ms_TIM2_Callback()
{
    init_finished++;
    if(init_finished > 2000)
        start_flag = 1;
    //留接口给：判断设备存活情况

    /****************************** 交互层回调函数 1ms *****************************************/
    if(start_flag == 1)
    {
        /*code*/
        wheel_group.TIM_Calculate_PeriodElapsedCallback();

        /****************************** 驱动层回调函数 1ms *****************************************/ 
        //统一打包发送
        TIM_CAN_PeriodElapsedCallback();
    }
}
extern "C" void Task_Init()
{
    //CAN初始化参数及相应回调函数
    CAN_Init(&hcan1,System_Device_CAN1_Callback);
    CAN_Init(&hcan2,System_Device_CAN2_Callback);
    //定时器初始化回调函数
    TIM_Init(&htim2,Task1ms_TIM2_Callback);
    /********************************* 设备层初始化 *********************************/

    //设备层集成在交互层初始化中，没有显视地初始化

    /********************************* 交互层初始化 *********************************/

    wheel_group.Init();

    /********************************* 使能调度时钟 *********************************/

    HAL_TIM_Base_Start_IT(&htim2);
}