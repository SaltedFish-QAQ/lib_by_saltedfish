/***********************************************************************************************
文件名	sIIC.h
日期	2013.4.28
作者	张诗星	陈东辉
文件说明	I2C软件模拟驱动支持文件
修订说明:

2013.04.28  张诗星 初始版本
2013.08.10  张诗星 修订延时函数
***********************************************************************************************/

/*头文件--------------------------------------------------------------------------------------*/
#include "sIIC.h"
#include <LPC213X.H>
#include "IODefine.h"

void delay(unsigned long time);

//IIC操作延时
#define IIC_X_Delay() (delay(20))


/*用户配置------------------------------------------------------------------------------------*/

//操作端口配置

//SCK 时钟端口 空闲时高电平 下降沿输出数据 上升沿锁存输入数据 

//SCK输出高电平
#define IIC_SET_SCK_H() (IO0SET = OPORT_I2C_SCL)
//SCK输出低电平
#define IIC_SET_SCK_L() (IO0CLR = OPORT_I2C_SCL)
//读取SCK引脚状态 高电平 低电平
#define IIC_GET_SCK() (IO0PIN&OPORT_I2C_SCL)

//SDA输出高电平
#define IIC_SET_SDA_H() (IO0SET = OPORT_I2C_SDA)
//SDA输出低电平
#define IIC_SET_SDA_L() (IO0CLR = OPORT_I2C_SDA)
//读取SDA引脚状态
#define IIC_GET_SDA() (IO0PIN&OPORT_I2C_SDA)


/*函数定义------------------------------------------------------------------------------------*/ 

//初始化IIC总线
void IIC_Init(void)
{
	//端口初始化
	IIC_SET_SDA_H();				/*释放SDA*/
	IIC_SET_SCK_H();				/*释放SCK*/
}

//产生起始信号 并读取总线状态
//     		 ________    
// SCK:  	         |_____
//      	 ____     
// SDA:        |_________
IIC_ResultEnum IIC_Start(void)
{
	IIC_X_Delay();					/*延时2us*/
	IIC_SET_SDA_H();				/*拉高SDA*/
	IIC_SET_SCK_H();				/*拉高SCK*/
	IIC_X_Delay();					/*延时2us*/
	
	//判断总线状态 是否空闲
	if (!IIC_GET_SCK() || !IIC_GET_SDA())
		return IIC_ERROR_BUSY;	/*总线忙*/

	//产生起始标志
	IIC_SET_SDA_L();					/*拉低SDA 起始标志*/
	IIC_X_Delay();						/*延时2us*/
	IIC_SET_SCK_L();					/*拉低SCK*/
	return IIC_ERROR_NOERROR;	/*总线空闲*/
}

//产生停止标志
//     		     _________   
// SCK:  	____|	  
//      	        ______      
// SDA:   _______|
void IIC_Stop(void)
{
	IIC_SET_SCK_L();			/*拉低SCK*/
	IIC_SET_SDA_L();			/*拉低SDA*/
	IIC_X_Delay();				/*延时2us*/
	IIC_SET_SCK_H();			/*拉高SCK*/
	IIC_X_Delay();				/*延时2us*/
	IIC_SET_SDA_H();			/*拉高SDA 产生STOP信号*/
	IIC_X_Delay();				/*延时2us*/
}

//检查总线是否空闲 0空闲 1忙
uint8_t IIC_CheckBusy(void)
{
	IIC_SET_SDA_H();				/*拉高SDA*/
	IIC_SET_SCK_H();				/*拉高SCK*/
	IIC_X_Delay();					/*延时2us*/

	//判断总线状态 是否空闲
	if (!IIC_GET_SCK() || !IIC_GET_SDA()) 
	{return IIC_ERROR_BUSY;}			/*总线忙*/

	return IIC_ERROR_NOERROR;
}

//写入一个字节 返回ACK
IIC_ResultEnum IIC_WriteByte(uint8_t byteData)
{
	//最高位先发送
	uint8_t i=8,ack;

	IIC_SET_SCK_L();
	IIC_X_Delay();				/*延时2us*/

	while (i)
	{
		//从最高位输出数据
		if (byteData&0x80) {IIC_SET_SDA_H();}
		else {IIC_SET_SDA_L();}
		
		--i;
		IIC_X_Delay();					/*延时2us*/
		IIC_SET_SCK_H();				/*上升沿锁存数据*/
		
		byteData<<=1;
		IIC_X_Delay();				/*延时2us*/
		IIC_SET_SCK_L();
		IIC_X_Delay();				/*延时2us*/
	}

	//读取ACK
	IIC_SET_SDA_H();				/*总线拉高 读取数据*/
	IIC_X_Delay();					/*延时2us*/
	IIC_SET_SCK_H();				/*产生ACK时钟*/
	IIC_X_Delay();					/*延时2us*/
	ack=IIC_GET_SDA();			/*读取ACK*/
	IIC_SET_SCK_L();	
	IIC_X_Delay();					/*延时2us*/

	if (IIC_ACK != ack) {return IIC_ERROR_NOACK;}
	return IIC_ERROR_NOERROR;
}

//读取一个字节 并写入ACK 0产生ACK 1无ACK
uint8_t IIC_ReadByte(uint8_t ack)
{
	//最高位先接收
	uint8_t i=8,temp=0;
	IIC_SET_SCK_L();
	IIC_SET_SDA_H();				/*拉高SDA以读出数据*/

	IIC_X_Delay();					/*延时2us*/
	while (i)
	{
		IIC_SET_SCK_H();			/*拉高SCK*/
		
		temp <<= 1;
		IIC_X_Delay();				/*延时2us*/
		if (IIC_GET_SDA()) {temp |= 0x01;}
		--i;
		IIC_SET_SCK_L();			/*拉低SCK*/
		IIC_X_Delay();				/*延时2us*/
	}

	//输出ACK
	if (IIC_ACK==ack) {IIC_SET_SDA_L();}		/*输出ACK*/	
	else {IIC_SET_SDA_H();}
	
	IIC_X_Delay();			/*延时2us*/
	IIC_SET_SCK_H();		/*产生ACK 时钟*/
	IIC_X_Delay();			/*延时2us*/
	IIC_SET_SCK_L();	
	IIC_X_Delay();			/*延时2us*/
	
	return temp;
}


//写缓冲区
IIC_ResultEnum IIC_WriteBuff(uint8_t* ptr, uint8_t len)
{
	if (!len) return IIC_ERROR_NOACK;
	
	while (len--)
	{
		//写入字节时从机无ACK 返回错误代码
		if (IIC_ERROR_NOERROR != IIC_WriteByte(*ptr)) return IIC_ERROR_NOACK;
		++ptr;
	}

	return IIC_ERROR_NOERROR;
}

//读缓冲区
IIC_ResultEnum IIC_ReadBuff(uint8_t * ptr,uint8_t len)
{
	if (!len) return IIC_ERROR_NOACK;
	
	while (len--)
	{
		*ptr = IIC_ReadByte(IIC_ACK);	/*读取数据并ACK*/
		++ptr;												/*指向下个数据*/
	}
	*ptr = IIC_ReadByte(IIC_NOACK);		/*最后一个字节无ACK*/
	return IIC_ERROR_NOERROR;
}


