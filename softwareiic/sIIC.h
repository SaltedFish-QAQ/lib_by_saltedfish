/***********************************************************************************************
文件名	sI2C.h
日期	2013.4.28
作者	张诗星	陈东辉
文件说明	I2C软件模拟驱动支持头文件
修订说明:

2013.04.28  张诗星 	初始版本
2014.07.23	张诗星	修订SCK SDA接口方式
***********************************************************************************************/

#ifndef _LIB_SOFT_IIC_H_
#define _LIB_SOFT_IIC_H_

/*头文件--------------------------------------------------------------------------------------*/
#include <stdint.h>

/*宏定义--------------------------------------------------------------------------------------*/

//错误码定义
typedef enum
{
	IIC_ERROR_NOERROR   = 0,			/* 无错误 */
	IIC_ERROR_NOACK     = 1,			/* 无ACK */
	IIC_ERROR_BUSY		= 2,			/* 总线忙 */
	IIC_ERROR_TIMEOUT   = 3,			/* 超时错误 */
}IIC_ResultEnum;

/*常量定义------------------------------------------------------------------------------------*/

#define IIC_ACK 			0				/* ACK */
#define IIC_NOACK			1				/* NO ACK */


/*接口函数------------------------------------------------------------------------------------*/

//初始化IIC的IO口
void IIC_Port_Init(void);

//初始化IIC总线
void IIC_Init(void);

//产生起始信号 并读取总线忙状态
IIC_ResultEnum IIC_Start(void);

//停止信号
void IIC_Stop(void);

//写入一个字节 返回ACK 0为ACK 否则为NOACK
IIC_ResultEnum IIC_WriteByte(uint8_t byteData);

//读取一个字节 并写入ACK 0为ACK 否则为NOACK
uint8_t IIC_ReadByte(uint8_t ack);

//写缓冲区 每写入一字节写ACK
IIC_ResultEnum IIC_WriteBuff(uint8_t* ptr, uint8_t len);

//读缓冲区 最后一字节无ACK
IIC_ResultEnum IIC_ReadBuff(uint8_t* ptr,uint8_t len);

//检查总线是否空闲 0空闲 1忙
uint8_t IIC_CheckBusy(void);

#endif


