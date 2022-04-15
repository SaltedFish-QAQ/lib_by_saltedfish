
#ifndef _USART_HMI_H_
#define _USART_HMI_H_

// *****************************************************************************************
// ͷ�ļ�

#include <stdarg.h>

// *****************************************************************************************
// ���Ͷ���

typedef enum
{
    HMI_STATE_OK,       ///< ���Գɹ�״̬
    HMI_STATE_BUSY,     ///< ������״̬
    HMI_STATE_ALM       ///< ���Դ���
}HMIState_Enum;

// *****************************************************************************************
// �ӿڶ���

void HMI_Init(void);
void HMI_SetState(HMIState_Enum cmd);
void HMI_VPrintf(const char* format, va_list arg);
void HMI_Printf(const char* format, ...);
void HMI_ClearScreen(void);

#endif

