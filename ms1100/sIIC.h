/***********************************************************************************************
�ļ���	sI2C.h
����	2013.4.28
����	��ʫ��	�¶���
�ļ�˵��	I2C���ģ������֧��ͷ�ļ�
�޶�˵��:

2013.04.28  ��ʫ�� 	��ʼ�汾
2014.07.23	��ʫ��	�޶�SCK SDA�ӿڷ�ʽ
***********************************************************************************************/

#ifndef _LIB_SOFT_IIC_H_
#define _LIB_SOFT_IIC_H_

/*ͷ�ļ�--------------------------------------------------------------------------------------*/
#include <stdint.h>

/*�궨��--------------------------------------------------------------------------------------*/

//�����붨��
typedef enum
{
	IIC_ERROR_NOERROR   = 0,			/* �޴��� */
	IIC_ERROR_NOACK     = 1,			/* ��ACK */
	IIC_ERROR_BUSY		= 2,			/* ����æ */
	IIC_ERROR_TIMEOUT   = 3,			/* ��ʱ���� */
}IIC_ResultEnum;

/*��������------------------------------------------------------------------------------------*/

#define IIC_ACK 			0				/* ACK */
#define IIC_NOACK			1				/* NO ACK */


/*�ӿں���------------------------------------------------------------------------------------*/

//��ʼ��IIC��IO��
void IIC_Port_Init(void);

//��ʼ��IIC����
void IIC_Init(void);

//������ʼ�ź� ����ȡ����æ״̬
IIC_ResultEnum IIC_Start(void);

//ֹͣ�ź�
void IIC_Stop(void);

//д��һ���ֽ� ����ACK 0ΪACK ����ΪNOACK
IIC_ResultEnum IIC_WriteByte(uint8_t byteData);

//��ȡһ���ֽ� ��д��ACK 0ΪACK ����ΪNOACK
uint8_t IIC_ReadByte(uint8_t ack);

//д������ ÿд��һ�ֽ�дACK
IIC_ResultEnum IIC_WriteBuff(uint8_t* ptr, uint8_t len);

//�������� ���һ�ֽ���ACK
IIC_ResultEnum IIC_ReadBuff(uint8_t* ptr,uint8_t len);

//��������Ƿ���� 0���� 1æ
uint8_t IIC_CheckBusy(void);

#endif


