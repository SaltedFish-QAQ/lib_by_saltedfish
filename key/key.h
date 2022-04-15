#ifndef _KEY_H_
#define _KEY_H_

#include <stdint.h>
#include "stm32f10x.h"

typedef struct ExterPort_s
{
    GPIO_TypeDef*    port;
    uint16_t        pin;
}ExterPort_t;

// typedef enum
// {
//     Bit_RESET = 0,
//     Bit_SET
// } Bit_OperateType;
typedef enum 
{
   lowlevel = 0,    //低电平
   highlevel,       //高电平
}ExterPortState;

typedef uint8_t (*ExterPort_Check)(ExterPort_t);
typedef void (*ExterPort_OutPut)(ExterPort_t, ExterPortState);

typedef enum 
{
   fallingedge,     //下降沿
   risingedge,      //上升沿
   longpress,       //长按
   press,           //按下
   nopress,         //松开
}KeyState;

typedef struct ExterPortObject
{
    ExterPort_t exterPort;           //外部端口号
    ExterPort_Check exterPort_Check;    //端口检测函数
    ExterPort_OutPut exterPort_outPut;  //端口输出函数
    ExterPortState state;               //端口当前状态
}ExterPortObject;


typedef struct KeyObject
{
    ExterPortObject exterPortObject;    //端口对象
    KeyState state;                     //按键状态
    uint16_t pressTimer;                //按键定时器
    uint16_t longPressDelay;            //长按延时
}KeyObject;

extern ExterPort_t gStartPort;

void ExterPort_HWInit(void);
void ExterPortObject_Init(ExterPortObject* pobj, ExterPort_t exterPort);
void KeyObject_Init(KeyObject* pobj, ExterPort_t exterPort);
void KeyScanInput(KeyObject* pobj);

#endif
