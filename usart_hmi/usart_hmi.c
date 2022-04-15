
// *****************************************************************************************
// ͷ�ļ�

#include <stdio.h>
#include "usart_hmi.h"
#include "stm32f10x.h"

// *****************************************************************************************
// �ڲ�����

#define CMD_ACTIVE  "\xFF\xFF\xFF"

// ���ڳ�ʼ��
static void _HmiSerialInit(uint32_t baudRate)
{
    USART_InitTypeDef USART_InitStructure;
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3 | RCC_APB2Periph_GPIOB, ENABLE);
    
    GPIO_Init(GPIOB, &(GPIO_InitTypeDef){GPIO_Pin_10, GPIO_Speed_2MHz, GPIO_Mode_AF_PP});   // UART1-TX

    USART_InitStructure.USART_BaudRate = baudRate;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Tx;
    
    USART_Init(USART3, &USART_InitStructure);
    USART_Cmd(USART3, ENABLE);
}

// *****************************************************************************************
// �ӿں���

/// \brief ��������øú����Գ�ʼ��HMI��ʾ��
///
/// \return void
void HMI_Init(void)
{
    _HmiSerialInit(9600);

    fprintf(stderr, CMD_ACTIVE);
    fprintf(stderr, CMD_ACTIVE);

    HMI_ClearScreen();
}

/// \brief �������������ʾ
///
/// \return void
void HMI_ClearScreen(void)
{
    fprintf(stderr, "t0.txt=\"\"" CMD_ACTIVE);
}

void HMI_VPrintf(const char* format, va_list arg)
{
    fprintf(stderr, "t0.txt+=\"");
    vfprintf(stderr, format, arg);
    fprintf(stderr, "\"" CMD_ACTIVE);
}

/// \brief ��HMI�д�ӡһ���ַ���
///
/// \details �ַ�����һֱ׷�ӵ�������ʾ����
///          ֱ������"HMI_ClearScreen"��������
/// \param format ��ʽ���ַ���
/// \return void
void HMI_Printf(const char* format, ...)
{
    va_list argp;
    va_start(argp, format);

    HMI_VPrintf(format, argp);

    va_end(argp);
}

/// \brief ����HMI״̬������OK��æ�л��Ǵ���
///
/// \param astate ״̬
/// \return void
void HMI_SetState(HMIState_Enum astate)
{
    const char* pcmd;
    if (HMI_STATE_OK == astate)
    {
        pcmd = "p0.pic=0" CMD_ACTIVE;
    }
    else if (HMI_STATE_BUSY == astate)
    {
        pcmd = "p0.pic=1" CMD_ACTIVE;
    }
    else
    {
        pcmd = "p0.pic=2" CMD_ACTIVE "play 1,0,0" CMD_ACTIVE;
    }

    fprintf(stderr, pcmd);
}
//*
// printf�ض���
//#include "SEGGER_RTT.h"  
int fputc(int ch, FILE* stream) 
{
    if (stderr == stream)
    {
        while(USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET);
        USART_SendData(USART3, ch);
    }
    else
    {
        //if ((char)ch == '\n')
        //   SEGGER_RTT_PutChar(0, '\r'); 
        //SEGGER_RTT_PutChar(0, ch);
    }
    return ch;
}
//*/

