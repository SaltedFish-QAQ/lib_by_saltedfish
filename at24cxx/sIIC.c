/***********************************************************************************************
�ļ���	sIIC.h
����	2013.4.28
����	��ʫ��	�¶���
�ļ�˵��	I2C���ģ������֧���ļ�
�޶�˵��:

2013.04.28  ��ʫ�� ��ʼ�汾
2013.08.10  ��ʫ�� �޶���ʱ����
***********************************************************************************************/

/*ͷ�ļ�--------------------------------------------------------------------------------------*/
#include "sIIC.h"
#include <LPC213X.H>
#include "IODefine.h"

void delay(unsigned long time);

//IIC������ʱ
#define IIC_X_Delay() (delay(20))


/*�û�����------------------------------------------------------------------------------------*/

//�����˿�����

//SCK ʱ�Ӷ˿� ����ʱ�ߵ�ƽ �½���������� ������������������ 

//SCK����ߵ�ƽ
#define IIC_SET_SCK_H() (IO0SET = OPORT_I2C_SCL)
//SCK����͵�ƽ
#define IIC_SET_SCK_L() (IO0CLR = OPORT_I2C_SCL)
//��ȡSCK����״̬ �ߵ�ƽ �͵�ƽ
#define IIC_GET_SCK() (IO0PIN&OPORT_I2C_SCL)

//SDA����ߵ�ƽ
#define IIC_SET_SDA_H() (IO0SET = OPORT_I2C_SDA)
//SDA����͵�ƽ
#define IIC_SET_SDA_L() (IO0CLR = OPORT_I2C_SDA)
//��ȡSDA����״̬
#define IIC_GET_SDA() (IO0PIN&OPORT_I2C_SDA)


/*��������------------------------------------------------------------------------------------*/ 

//��ʼ��IIC����
void IIC_Init(void)
{
	//�˿ڳ�ʼ��
	IIC_SET_SDA_H();				/*�ͷ�SDA*/
	IIC_SET_SCK_H();				/*�ͷ�SCK*/
}

//������ʼ�ź� ����ȡ����״̬
//     		 ________    
// SCK:  	         |_____
//      	 ____     
// SDA:        |_________
IIC_ResultEnum IIC_Start(void)
{
	IIC_X_Delay();					/*��ʱ2us*/
	IIC_SET_SDA_H();				/*����SDA*/
	IIC_SET_SCK_H();				/*����SCK*/
	IIC_X_Delay();					/*��ʱ2us*/
	
	//�ж�����״̬ �Ƿ����
	if (!IIC_GET_SCK() || !IIC_GET_SDA())
		return IIC_ERROR_BUSY;	/*����æ*/

	//������ʼ��־
	IIC_SET_SDA_L();					/*����SDA ��ʼ��־*/
	IIC_X_Delay();						/*��ʱ2us*/
	IIC_SET_SCK_L();					/*����SCK*/
	return IIC_ERROR_NOERROR;	/*���߿���*/
}

//����ֹͣ��־
//     		     _________   
// SCK:  	____|	  
//      	        ______      
// SDA:   _______|
void IIC_Stop(void)
{
	IIC_SET_SCK_L();			/*����SCK*/
	IIC_SET_SDA_L();			/*����SDA*/
	IIC_X_Delay();				/*��ʱ2us*/
	IIC_SET_SCK_H();			/*����SCK*/
	IIC_X_Delay();				/*��ʱ2us*/
	IIC_SET_SDA_H();			/*����SDA ����STOP�ź�*/
	IIC_X_Delay();				/*��ʱ2us*/
}

//��������Ƿ���� 0���� 1æ
uint8_t IIC_CheckBusy(void)
{
	IIC_SET_SDA_H();				/*����SDA*/
	IIC_SET_SCK_H();				/*����SCK*/
	IIC_X_Delay();					/*��ʱ2us*/

	//�ж�����״̬ �Ƿ����
	if (!IIC_GET_SCK() || !IIC_GET_SDA()) 
	{return IIC_ERROR_BUSY;}			/*����æ*/

	return IIC_ERROR_NOERROR;
}

//д��һ���ֽ� ����ACK
IIC_ResultEnum IIC_WriteByte(uint8_t byteData)
{
	//���λ�ȷ���
	uint8_t i=8,ack;

	IIC_SET_SCK_L();
	IIC_X_Delay();				/*��ʱ2us*/

	while (i)
	{
		//�����λ�������
		if (byteData&0x80) {IIC_SET_SDA_H();}
		else {IIC_SET_SDA_L();}
		
		--i;
		IIC_X_Delay();					/*��ʱ2us*/
		IIC_SET_SCK_H();				/*��������������*/
		
		byteData<<=1;
		IIC_X_Delay();				/*��ʱ2us*/
		IIC_SET_SCK_L();
		IIC_X_Delay();				/*��ʱ2us*/
	}

	//��ȡACK
	IIC_SET_SDA_H();				/*�������� ��ȡ����*/
	IIC_X_Delay();					/*��ʱ2us*/
	IIC_SET_SCK_H();				/*����ACKʱ��*/
	IIC_X_Delay();					/*��ʱ2us*/
	ack=IIC_GET_SDA();			/*��ȡACK*/
	IIC_SET_SCK_L();	
	IIC_X_Delay();					/*��ʱ2us*/

	if (IIC_ACK != ack) {return IIC_ERROR_NOACK;}
	return IIC_ERROR_NOERROR;
}

//��ȡһ���ֽ� ��д��ACK 0����ACK 1��ACK
uint8_t IIC_ReadByte(uint8_t ack)
{
	//���λ�Ƚ���
	uint8_t i=8,temp=0;
	IIC_SET_SCK_L();
	IIC_SET_SDA_H();				/*����SDA�Զ�������*/

	IIC_X_Delay();					/*��ʱ2us*/
	while (i)
	{
		IIC_SET_SCK_H();			/*����SCK*/
		
		temp <<= 1;
		IIC_X_Delay();				/*��ʱ2us*/
		if (IIC_GET_SDA()) {temp |= 0x01;}
		--i;
		IIC_SET_SCK_L();			/*����SCK*/
		IIC_X_Delay();				/*��ʱ2us*/
	}

	//���ACK
	if (IIC_ACK==ack) {IIC_SET_SDA_L();}		/*���ACK*/	
	else {IIC_SET_SDA_H();}
	
	IIC_X_Delay();			/*��ʱ2us*/
	IIC_SET_SCK_H();		/*����ACK ʱ��*/
	IIC_X_Delay();			/*��ʱ2us*/
	IIC_SET_SCK_L();	
	IIC_X_Delay();			/*��ʱ2us*/
	
	return temp;
}


//д������
IIC_ResultEnum IIC_WriteBuff(uint8_t* ptr, uint8_t len)
{
	if (!len) return IIC_ERROR_NOACK;
	
	while (len--)
	{
		//д���ֽ�ʱ�ӻ���ACK ���ش������
		if (IIC_ERROR_NOERROR != IIC_WriteByte(*ptr)) return IIC_ERROR_NOACK;
		++ptr;
	}

	return IIC_ERROR_NOERROR;
}

//��������
IIC_ResultEnum IIC_ReadBuff(uint8_t * ptr,uint8_t len)
{
	if (!len) return IIC_ERROR_NOACK;
	
	while (len--)
	{
		*ptr = IIC_ReadByte(IIC_ACK);	/*��ȡ���ݲ�ACK*/
		++ptr;												/*ָ���¸�����*/
	}
	*ptr = IIC_ReadByte(IIC_NOACK);		/*���һ���ֽ���ACK*/
	return IIC_ERROR_NOERROR;
}


