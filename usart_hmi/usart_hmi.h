
#ifndef _USART_HMI_H_
#define _USART_HMI_H_

// *****************************************************************************************
// 头文件

#include <stdarg.h>

// *****************************************************************************************
// 类型定义

typedef enum
{
    HMI_STATE_OK,       ///< 测试成功状态
    HMI_STATE_BUSY,     ///< 测试中状态
    HMI_STATE_ALM       ///< 测试错误
}HMIState_Enum;

// *****************************************************************************************
// 接口定义

void HMI_Init(void);
void HMI_SetState(HMIState_Enum cmd);
void HMI_VPrintf(const char* format, va_list arg);
void HMI_Printf(const char* format, ...);
void HMI_ClearScreen(void);

#endif

