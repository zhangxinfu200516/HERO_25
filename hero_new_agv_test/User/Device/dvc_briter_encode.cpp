#include "dvc_briter_encode.h"
/**
 * @brief 分配CAN发送缓冲区
 *
 * @param hcan CAN编号
 * @param __CAN_ID CAN ID
 * @return uint8_t* 缓冲区指针
 */
uint8_t *allocate_tx_data(CAN_HandleTypeDef *hcan, Enum_BRITER_ENCODER_ID __CAN_ID)
{
    uint8_t *tmp_tx_data_ptr;
    if (hcan == &hcan1)
    {
        switch (__CAN_ID)
        {
        case (A_ENCODER_ID):
        {
            tmp_tx_data_ptr = &(CAN1_0x0A_Tx_Data[0]);
        }
        break;
        case (B_ENCODER_ID):
        {
            tmp_tx_data_ptr = &(CAN1_0x0B_Tx_Data[0]);
        }
        break;
        case (C_ENCODER_ID):
        {
            tmp_tx_data_ptr = &(CAN1_0x0C_Tx_Data[0]);
        }
        break;
        case (D_ENCODER_ID):
        {
            tmp_tx_data_ptr = &(CAN1_0x0D_Tx_Data[0]);
        }
        break;
        }
    }
    
    return (tmp_tx_data_ptr);
}
/**
 * @brief 电机初始化
 *
 * @param hcan CAN编号
 * @param __CAN_ID CAN ID
 * @param  __Gear_ratio   默认为1
 * 已经设置布瑞特编码器波特率为1MHz
 */
void Class_Briter_Encoder::Init(CAN_HandleTypeDef *__hcan, Enum_BRITER_ENCODER_ID __CAN_ID, 
    Enum_Briter_Encoder_Control_Method __Control_Method,Enum_Briter_Encoder_Baud __baud,
    Enum_Briter_Encoder_Increment_Driection __Driection,Enum_Briter_Encoder_Callback_Mode __mode)
{
    if (__hcan->Instance == CAN1)
    {
        CAN_Manage_Object = &CAN1_Manage_Object;
    }
    else if (__hcan->Instance == CAN2)
    {
        CAN_Manage_Object = &CAN2_Manage_Object;
    }
    CAN_ID = __CAN_ID;
    briter_encoder_controlmethod = __Control_Method;
    briter_encoder_baud = __baud;
    briter_encoder_increment_driection = __Driection;
    briter_encoder_callback_mode = __mode;
    memcpy(allocate_tx_data(__hcan, __CAN_ID),&briter_encoder_txcommand,8);
}
uint8_t zero = 0;
void Class_Briter_Encoder::Briter_Ecoder_Request_Tatal_Angle()
{
    briter_encoder_txcommand.length = 0x04;
    briter_encoder_txcommand.encoder_address = Get_Briter_Encoder_ID();
    briter_encoder_txcommand.command_code = Get_BriterEncoder_ControlMethod();
    memcpy(briter_encoder_txcommand.data,&zero,5);
}
void Class_Briter_Encoder::Briter_Encoder_Set_CAN_ID()
{
    briter_encoder_txcommand.length = 0x04;
    briter_encoder_txcommand.encoder_address = Get_Briter_Encoder_ID();
    briter_encoder_txcommand.command_code = Get_BriterEncoder_ControlMethod();
    briter_encoder_txcommand.data[0] = Get_Briter_Encoder_ID();
    memcpy(&(briter_encoder_txcommand.data[1]),&zero,4);
}
void Class_Briter_Encoder::Briter_Encoder_Set_Baud()
{
    briter_encoder_txcommand.length = 0x04;
    briter_encoder_txcommand.encoder_address = Get_Briter_Encoder_ID();
    briter_encoder_txcommand.command_code = Get_BriterEncoder_ControlMethod();
    briter_encoder_txcommand.data[0] = Get_Briter_Encoder_Baud();
    memcpy(&(briter_encoder_txcommand.data[1]),&zero,4);
}
void Class_Briter_Encoder::Briter_Encoder_Set_Encoder_Mode()
{
    briter_encoder_txcommand.length = 0x04;
    briter_encoder_txcommand.encoder_address = Get_Briter_Encoder_ID();
    briter_encoder_txcommand.command_code = Get_BriterEncoder_ControlMethod();
    briter_encoder_txcommand.data[0] = Get_BriterEncoder_ControlMethod();//自动回发
    memcpy(&(briter_encoder_txcommand.data[1]),&zero,4);
}
void Class_Briter_Encoder::Briter_Encoder_Set_Callback_Period()
{
    briter_encoder_txcommand.length = 0x04;
    briter_encoder_txcommand.encoder_address = Get_Briter_Encoder_ID();
    briter_encoder_txcommand.command_code = Get_BriterEncoder_ControlMethod();
    briter_encoder_txcommand.data[0] = 0X03E8;//设定自动回传时间：0X03E8（1000 微秒）
    memcpy(&(briter_encoder_txcommand.data[1]),&zero,4);
}
void Class_Briter_Encoder::Briter_Encoder_Set_Now_Postion_to_Zero()
{
    briter_encoder_txcommand.length = 0x04;
    briter_encoder_txcommand.encoder_address = Get_Briter_Encoder_ID();
    briter_encoder_txcommand.command_code = Get_BriterEncoder_ControlMethod();
    memcpy(briter_encoder_txcommand.data,&zero,5);
}
void Class_Briter_Encoder::Briter_Encoder_Set_Increment_Direction()
{
    briter_encoder_txcommand.length = 0x04;
    briter_encoder_txcommand.encoder_address = Get_Briter_Encoder_ID();
    briter_encoder_txcommand.command_code = Get_BriterEncoder_ControlMethod();
    briter_encoder_txcommand.data[0] = Get_Briter_Encoder_Increment_Driection();//设置编码器值递增方向:逆时针
    memcpy(&(briter_encoder_txcommand.data[1]),&zero,4);
}
void Class_Briter_Encoder::Briter_Encoder_Set_Now_Position_To_Middle()
{
    briter_encoder_txcommand.length = 0x04;
    briter_encoder_txcommand.encoder_address = Get_Briter_Encoder_ID();
    briter_encoder_txcommand.command_code = Get_BriterEncoder_ControlMethod();
    briter_encoder_txcommand.data[0] = 0x01;//设置当前位置为中点
    memcpy(&(briter_encoder_txcommand.data[1]),&zero,4);
}
void Class_Briter_Encoder::Briter_Encoder_Set_Now_Position_To_Specific()
{
    uint8_t tmp_data[5] = {0x00,0x01,0x23,0x45,0x00};
    briter_encoder_txcommand.length = 0x07;
    briter_encoder_txcommand.encoder_address = Get_Briter_Encoder_ID();
    briter_encoder_txcommand.command_code = Get_BriterEncoder_ControlMethod();
    memcpy(briter_encoder_txcommand.data,tmp_data,5);//设定编码器值：0X00012345（十进制：74565）
}
void Class_Briter_Encoder::Briter_Encoder_Set_Now_Value_To_Five_Round_Value()
{
    briter_encoder_txcommand.length = 0x04;
    briter_encoder_txcommand.encoder_address = Get_Briter_Encoder_ID();
    briter_encoder_txcommand.command_code = Get_BriterEncoder_ControlMethod();
    briter_encoder_txcommand.data[0] = 0x01;
    memcpy(&(briter_encoder_txcommand.data[1]),&zero,4);
}
void Class_Briter_Encoder::Data_Process()
{
    switch (briter_encoder_controlmethod)
    { 
    case get_total_angle:
        memcpy(&briter_encoder_callbackdata,CAN_Manage_Object->Rx_Buffer.Data,8);
        if(briter_encoder_callbackdata.length == 0x07)//判断数据长度是否正确
        {
            memcpy(&multi_turn_encoder,briter_encoder_callbackdata.data,sizeof(multi_turn_encoder));
            // 多圈角度（编码器50圈，对应舵5圈）变为单圈角度（编码器10圈，对应舵1圈）
            single_turn_encoder = multi_turn_encoder % lsbs_per_part_round;
            // 多圈角度映射到 统一的角度分辨率（也是通信传输时的分辨率）
            standard_encoder = single_turn_encoder * PROTOCOL_POSITION_LSBS /  lsbs_per_part_round;
        }

        break;
    case set_can_id:
        // code
        break;
    default:
        break;
    }
    

}
void Class_Briter_Encoder::CAN_RxCpltCallback(uint8_t *Rx_Data)
{
    //滑动窗口, 判断电机是否在线
    Flag += 1;

    Data_Process();
}

void Class_Briter_Encoder::Briter_Encoder_CAN_Tx_Callback()
{
    switch (briter_encoder_controlmethod)
    {
    case get_total_angle:
        /* code */
        Briter_Ecoder_Request_Tatal_Angle();
        break;
    case set_can_id:
        /* code */
        Briter_Encoder_Set_CAN_ID();
        break;
    case set_can_baud_rate:
        /* code */
        Briter_Encoder_Set_Baud();
        break;
    case set_callback_mode:
        /* code */
        Briter_Encoder_Set_Encoder_Mode();
        break;
    case set_callback_period:
        /* code */
        Briter_Encoder_Set_Callback_Period();
        break;
    case set_current_pos_zero_pos:
        /* code */
        Briter_Encoder_Set_Now_Postion_to_Zero();
        break;
    case set_increment_direction:
        /* code */
        Briter_Encoder_Set_Increment_Direction();
        break;
    case set_current_pos_mid_pos:
        /* code */
        Briter_Encoder_Set_Now_Position_To_Middle();
        break;
    case set_current_pos_specific_value:
        /* code */
        Briter_Encoder_Set_Now_Position_To_Specific();
        break;
    case set_current_pos_5round_value:
        /* code */
        Briter_Encoder_Set_Now_Value_To_Five_Round_Value();
        break;
    default:
        break;
    }
}
void Class_Briter_Encoder::TIM_Briter_Encoder_PeriodElapsedCallback()
{
    
    Briter_Encoder_CAN_Tx_Callback();
}