#ifndef _DVC_BRITER_ENCODER_H_
#define _DVC_BRITER_ENCODER_H_
#ifdef __cplusplus
#include "struct_typedef.h"
#include "drv_can.h"

// 统一的角度分辨率。从零点开始CW方向，将角度等分为 PROTOCOL_POSITION_LSBS 份。
#define PROTOCOL_POSITION_LSBS	8191

//布瑞特编码器的CAN指令列表
enum  Enum_Briter_Encoder_Control_Method
{  
    control_disable,
    get_total_angle = 0x01,  
    set_can_id = 0x02,  
    set_can_baud_rate = 0x03,  
    set_callback_mode = 0x04,  
    set_callback_period = 0x05,  
    set_current_pos_zero_pos = 0x06,  
    set_increment_direction = 0x07,  
    set_current_pos_mid_pos = 0x0C,  
    set_current_pos_specific_value = 0x0D,  
    set_current_pos_5round_value = 0x0F  
};
//布瑞特编码器的指令结构
/*【数据长度】 + 【编码器地址】+ 【指令 FUNC】+ 【数据 DATA】
数据长度：1 字节，数据范围 0~8，包括本身、编码器地址、指令 FUNC、数据
DATA 的字节总数。注意：这个数据长度不同于帧信息的数据长度。
编码器地址：编码器的 CAN 节点地址，1 字节
指令 FUNC：通讯的功能码，1 字节
数据 DATA：指令所带的数据，0~4 字节*/
struct struct_briter_encoder_command
{
	uint8_t length;
	uint8_t encoder_address;
	uint8_t command_code;
	uint8_t data[5];
}__attribute__((packed));
struct Struct_Briter_Encoder_CallbackData
{
	uint8_t length;
	uint8_t encoder_address;
	uint8_t command_code;
	uint8_t data[5];
}__attribute__((packed));
//布瑞特编码器ID
enum  Enum_BRITER_ENCODER_ID
{
    A_ENCODER_ID = 0X0A,
    B_ENCODER_ID = 0X0B,
    C_ENCODER_ID = 0X0C,
    D_ENCODER_ID = 0X0D,

};
enum  Enum_Briter_Encoder_Baud
{
    Encoder_Baud_500K = 0x00,
    Encoder_Baud_1M = 0x01,
    Encoder_Baud_250K = 0x02,
    Encoder_Baud_125K = 0x03,
    Encoder_Baud_100K = 0x04,

};

enum Enum_Briter_Encoder_Increment_Driection
{
    briter_encoder_increment_directon_cw = 0x00,
    briter_encoder_increment_directon_ccw = 0x01,//顺时针

};
enum Enum_Briter_Encoder_Callback_Mode
{
    brier_encoder_set_callback_request,
    brier_encoder_set_callback_periodical = 0xAA,//自动回发
};
#define A_ENCODER_ZERO_POSION 0x1e80U
#define B_ENCODER_ZERO_POSION 0x1822U
#define C_ENCODER_ZERO_POSION 0x1d62U
#define D_ENCODER_ZERO_POSION 0x01acU	

class Class_Briter_Encoder
{
public:
    inline Enum_Briter_Encoder_Control_Method Get_BriterEncoder_ControlMethod();
    inline Enum_BRITER_ENCODER_ID Get_Briter_Encoder_ID();
    inline Enum_Briter_Encoder_Baud Get_Briter_Encoder_Baud();
    inline Enum_Briter_Encoder_Increment_Driection Get_Briter_Encoder_Increment_Driection();
    inline Enum_Briter_Encoder_Callback_Mode Get_Briter_Encoder_Callback_Mode();
    inline Enum_Briter_Encoder_Control_Method Set_BriterEncoder_ControlMethod(Enum_Briter_Encoder_Control_Method __Control_Method);
    inline int32_t Get_Single_Turn_Encoder();
    inline uint16_t Get_Lsbs_Per_Part_Round();
    void Init(CAN_HandleTypeDef *__hcan, Enum_BRITER_ENCODER_ID __CAN_ID, Enum_Briter_Encoder_Control_Method __Control_Method,Enum_Briter_Encoder_Baud __baud,
        Enum_Briter_Encoder_Increment_Driection __Driection = briter_encoder_increment_directon_cw,Enum_Briter_Encoder_Callback_Mode __mode = brier_encoder_set_callback_request);
    void CAN_RxCpltCallback(uint8_t *Rx_Data);
    void Briter_Encoder_CAN_Tx_Callback();
    void TIM_Briter_Encoder_PeriodElapsedCallback();
protected:
//初始化相关变量
    //绑定的CAN
    Struct_CAN_Manage_Object *CAN_Manage_Object;
    //编码器CAN_ID
    Enum_BRITER_ENCODER_ID CAN_ID;
    //指令发送缓冲区（控制布瑞特编码器的指令）
    struct_briter_encoder_command  briter_encoder_txcommand;
    //布瑞特编码器回调数据缓冲区
    Struct_Briter_Encoder_CallbackData  briter_encoder_callbackdata;
    

//常量
    uint16_t lsbs_per_encoder_round = 1024;
    uint8_t encoder_rounds_per_part_round = 5;
    uint16_t lsbs_per_part_round = 1024*5;
//内部变量
    //当前时刻的电机接收flag
    uint32_t Flag = 0;
    //前一时刻的电机接收flag
    uint32_t Pre_Flag = 0;

//读变量
    //电机状态
    //获取的编码器值
    int32_t multi_turn_encoder;
    int32_t single_turn_encoder;
    int16_t standard_encoder;
    //电机对外接口信息
    
//写变量

//读写变量
    //电机控制方式
    Enum_Briter_Encoder_Control_Method  briter_encoder_controlmethod;
    Enum_Briter_Encoder_Baud    briter_encoder_baud;
    Enum_Briter_Encoder_Increment_Driection briter_encoder_increment_driection;
    Enum_Briter_Encoder_Callback_Mode briter_encoder_callback_mode;
    //内部函数
    void Briter_Ecoder_Request_Tatal_Angle();
    void Briter_Encoder_Set_CAN_ID();
    void Briter_Encoder_Set_Baud();
    void Briter_Encoder_Set_Encoder_Mode();
    void Briter_Encoder_Set_Callback_Period();//ms
    void Briter_Encoder_Set_Now_Postion_to_Zero();
    void Briter_Encoder_Set_Increment_Direction();//0x00:顺时针 0x01:逆时针
    void Briter_Encoder_Set_Now_Position_To_Middle();
    void Briter_Encoder_Set_Now_Position_To_Specific();
    void Briter_Encoder_Set_Now_Value_To_Five_Round_Value();
    void Data_Process();
};
Enum_BRITER_ENCODER_ID Class_Briter_Encoder::Get_Briter_Encoder_ID()
{
    return (CAN_ID);
}

Enum_Briter_Encoder_Control_Method  Class_Briter_Encoder::Get_BriterEncoder_ControlMethod()
{
    return (briter_encoder_controlmethod);
}

Enum_Briter_Encoder_Control_Method  Class_Briter_Encoder::Set_BriterEncoder_ControlMethod(Enum_Briter_Encoder_Control_Method __Control_Method)
{
    briter_encoder_controlmethod = __Control_Method;
}

Enum_Briter_Encoder_Baud Class_Briter_Encoder::Get_Briter_Encoder_Baud()
{
    return (briter_encoder_baud);
}

Enum_Briter_Encoder_Increment_Driection Class_Briter_Encoder::Get_Briter_Encoder_Increment_Driection()
{
    return (briter_encoder_increment_driection);
}

Enum_Briter_Encoder_Callback_Mode Class_Briter_Encoder::Get_Briter_Encoder_Callback_Mode()
{
    return (briter_encoder_callback_mode);
}

int32_t Class_Briter_Encoder::Get_Single_Turn_Encoder()
{
    return (single_turn_encoder);
}

uint16_t Class_Briter_Encoder::Get_Lsbs_Per_Part_Round()
{
    return (lsbs_per_part_round);
}
#endif

#endif
