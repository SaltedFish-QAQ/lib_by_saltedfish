/***********************************************************************************************
�ļ���	AT24Cxx.C
����	2013.4.29
����	��ʫ��	�¶���
�ļ�˵��	AT24C02/08/16/32/64/256��ͷ�ļ�
�޶�˵��:

2013.4.29  ��ʫ�� ��ʼ�汾
***********************************************************************************************/

/*ͷ�ļ�--------------------------------------------------------------------------------------*/
#include "AT24Cxx.h"

/*�û�����------------------------------------------------------------------------------------*/

//AT24C02/AT24C04/AT24C08/AT24C16/AT24C32/AT24C64/AT24C128/AT24C256

//��ʼʱ��
//unsigned char AT24Cxx_IIC_Start(void)
#define AT24Cxx_IIC_Start() IIC_Start()
//����ʱ��
//void AT24Cxx_IIC_Stop(void)
#define AT24Cxx_IIC_Stop() IIC_Stop()
//��ʼ��
//void AT24Cxx_IIC_Init(void)
#define AT24Cxx_IIC_Init() IIC_Init()
//д����
// unsigned char IIC_WriteByte(unsigned char) ����д�������
#define AT24Cxx_IIC_Write(X) IIC_WriteByte(X)
//������
// unsigned char IIC_ReadByte(unsigned char) ����ָ���Ƿ����ACK
#define AT24Cxx_IIC_Read(X) IIC_ReadByte(X)

/*�궨��--------------------------------------------------------------------------------------*/

#define AT24Cxx_ADDRESS 		(0xa0|(AT24Cxx_AddrID&0x07)<<1)	/*����IIC��ַ*/
#define AT24Cxx_CMD_WRITE	(AT24Cxx_ADDRESS)								/*д����*/
#define AT24Cxx_CMD_READ 	(AT24Cxx_ADDRESS+1)							/*������*/

#ifndef AT24Cxx_TYPE						/*Ĭ����������*/
	#define AT24Cxx_TYPE AT24C02
#endif

/*�꺯��--------------------------------------------------------------------------------------*/

/*д��������ַ ��ȡACK���*/

//����������
#if AT24Cxx_TYPE>AT24C16				
		#define AT24Cxx_WriteADDr(ADDR,RESULT)																	\
		RESULT=AT24Cxx_IIC_Write(AT24Cxx_CMD_WRITE);				/*д��������ַ*/	\
		RESULT|=AT24Cxx_IIC_Write((uint8_t)(ADDR>>8));						/*д�ߵ�ַ*/			\
		RESULT|=AT24Cxx_IIC_Write((unsigned char)(ADDR));			/*д�͵�ַ*/
		
//����������	
#elif AT24Cxx_TYPE>AT24C02			
		#define AT24Cxx_WriteADDr(ADDR,RESULT)																		\
		RESULT=AT24Cxx_IIC_Write(AT24Cxx_CMD_WRITE+((uint8_t)((ADDR>>8)&0x07)<<1));	\
		RESULT|=AT24Cxx_IIC_Write((unsigned char)(ADDR));		/*д�͵�ַ*/
		
//����������
#else														
		#define AT24Cxx_WriteADDr(ADDR,RESULT)																\
		RESULT=AT24Cxx_IIC_Write(AT24Cxx_CMD_WRITE);												\
		RESULT|=AT24Cxx_IIC_Write((unsigned char)(ADDR));		/*д�͵�ַ*/
		
#endif


/*��������------------------------------------------------------------------------------------*/

//��ʼ������ ���ؽ��
IIC_ResultEnum AT24Cxx_Init(void)
{
	IIC_ResultEnum result;
	//��ʼ���ײ�IIC����
	AT24Cxx_IIC_Init();

	//��������Ƿ���Ӧ
	result = AT24Cxx_IIC_Start();	/* ��ʼ��־ */

	if (IIC_ERROR_NOERROR != result) 
	{AT24Cxx_IIC_Stop();return result;}
	//���߿���

	//��ȡ��ַ0���� ����ACKλ
	result = AT24Cxx_IIC_Write(AT24Cxx_ADDRESS);	/* д���ַ */

	AT24Cxx_IIC_Stop();				/* ֹͣ���� */
	return result;
}

//д��һ���ֽ� ��ַ���ֽ�Ϊ����Ӳ����ַ��ҳ��ַ ���ֽ�Ϊ�ֽڵ�ַ
IIC_ResultEnum AT24Cxx_WriteByte(uint16_t addr,uint8_t byteData)
{
	IIC_ResultEnum result=IIC_ERROR_NOERROR;
	
	result = AT24Cxx_IIC_Start();	/*��ʼ��־*/
	if (IIC_ERROR_NOERROR != result) 
	{AT24Cxx_IIC_Stop();return result;}
	
	AT24Cxx_WriteADDr(addr,result);	/*д���ַ����ȡӦ����*/
	AT24Cxx_IIC_Write(byteData);		/*д������*/
	AT24Cxx_IIC_Stop();							/*����ֹͣ��־*/

	return result;
}

//Ӧ���ѯ
IIC_ResultEnum AT24Cxx_QueryWriteBusy(void)
{
	IIC_ResultEnum result=IIC_ERROR_NOERROR;

	result = AT24Cxx_IIC_Start();	/* ��ʼ��־ */
	if (IIC_ERROR_NOERROR != result) 
	{AT24Cxx_IIC_Stop();return result;}

	result = AT24Cxx_IIC_Write(AT24Cxx_ADDRESS);	/* д���ַ */

	AT24Cxx_IIC_Stop();
	return result;
}

//��ȡ����
uint8_t AT24Cxx_ReadByte(uint16_t addr)
{
	IIC_ResultEnum result=IIC_ERROR_NOERROR;
	unsigned char temp;
	
	result = AT24Cxx_IIC_Start();	/* ��ʼ��־ */
	if (IIC_ERROR_NOERROR != result) 
	{AT24Cxx_IIC_Stop();return 0;}
	
	AT24Cxx_WriteADDr(addr,temp);	/* д���ַ����ȡӦ���� */
	AT24Cxx_IIC_Start();					/* ��ʼ��־ */
	AT24Cxx_IIC_Write(AT24Cxx_CMD_READ);	/* ������ */
	temp = AT24Cxx_IIC_Read(IIC_NOACK);		/* ��ȡ���� ��Ӧ�� */
	AT24Cxx_IIC_Stop();						/* ֹͣ��־ */
	return temp;
}

//д��ҳ����
IIC_ResultEnum AT24Cxx_WritePtr(uint16_t addr,const uint8_t* ptr,uint8_t len)
{
	IIC_ResultEnum result=IIC_ERROR_NOERROR;
	
	result = AT24Cxx_IIC_Start();	/* ��ʼ��־ */
	if (IIC_ERROR_NOERROR != result) 
	{AT24Cxx_IIC_Stop();return result;}
	
	AT24Cxx_WriteADDr(addr,result); /* д���ַ����ȡӦ���� */
	//д������
	while (len--)
	{
		AT24Cxx_IIC_Write(*ptr);	/* д������ */
		++ptr;										/* ָ���¸����� */
	}
	AT24Cxx_IIC_Stop();					/* ֹͣ��־ ʹ��д�� */
	return result;
}
//��ȡ���ݵ�������
IIC_ResultEnum AT24Cxx_ReadPtr(uint16_t addr,uint8_t* ptr,uint8_t len)
{
	IIC_ResultEnum result=IIC_ERROR_NOERROR;
	
	if (!len) {return IIC_ERROR_NOERROR;}		/* ��������ֱ���˳� */
	
	result = AT24Cxx_IIC_Start();						/* ��ʼ��־ */
	if (IIC_ERROR_NOERROR != result) 
	{AT24Cxx_IIC_Stop();return result;}
	
	AT24Cxx_WriteADDr(addr,result);				/* д���ַ����ȡӦ���� */
	result |= AT24Cxx_IIC_Start();				/* ��ʼ��־ */
	result |= AT24Cxx_IIC_Write(AT24Cxx_CMD_READ);	/* ������ */

	while (--len)
	{
		*ptr = AT24Cxx_IIC_Read(IIC_ACK);	/* ��ȡһ���ֽڲ�ACK */
		++ptr;														/* ָ����һ��λ�� */
	}
	*ptr = AT24Cxx_IIC_Read(IIC_NOACK);	/* ���һ���ֽ���ACK */

	AT24Cxx_IIC_Stop();									/* ֹͣ��־ ʹ��д�� */
	return result;
}

