/***********************************************************************************************
文件名	AT24Cxx.C
日期	2013.4.29
作者	张诗星	陈东辉
文件说明	AT24C02/08/16/32/64/256库头文件
修订说明:

2013.4.29  张诗星 初始版本
***********************************************************************************************/

/*头文件--------------------------------------------------------------------------------------*/
#include "AT24Cxx.h"

/*用户配置------------------------------------------------------------------------------------*/

//AT24C02/AT24C04/AT24C08/AT24C16/AT24C32/AT24C64/AT24C128/AT24C256

//起始时序
//unsigned char AT24Cxx_IIC_Start(void)
#define AT24Cxx_IIC_Start() IIC_Start()
//结束时序
//void AT24Cxx_IIC_Stop(void)
#define AT24Cxx_IIC_Stop() IIC_Stop()
//初始化
//void AT24Cxx_IIC_Init(void)
#define AT24Cxx_IIC_Init() IIC_Init()
//写操作
// unsigned char IIC_WriteByte(unsigned char) 返回写入错误码
#define AT24Cxx_IIC_Write(X) IIC_WriteByte(X)
//读操作
// unsigned char IIC_ReadByte(unsigned char) 参数指定是否产生ACK
#define AT24Cxx_IIC_Read(X) IIC_ReadByte(X)

/*宏定义--------------------------------------------------------------------------------------*/

#define AT24Cxx_ADDRESS 		(0xa0|(AT24Cxx_AddrID&0x07)<<1)	/*器件IIC地址*/
#define AT24Cxx_CMD_WRITE	(AT24Cxx_ADDRESS)								/*写命令*/
#define AT24Cxx_CMD_READ 	(AT24Cxx_ADDRESS+1)							/*读命令*/

#ifndef AT24Cxx_TYPE						/*默认器件类型*/
	#define AT24Cxx_TYPE AT24C02
#endif

/*宏函数--------------------------------------------------------------------------------------*/

/*写入器件地址 读取ACK结果*/

//高容量器件
#if AT24Cxx_TYPE>AT24C16				
		#define AT24Cxx_WriteADDr(ADDR,RESULT)																	\
		RESULT=AT24Cxx_IIC_Write(AT24Cxx_CMD_WRITE);				/*写入器件地址*/	\
		RESULT|=AT24Cxx_IIC_Write((uint8_t)(ADDR>>8));						/*写高地址*/			\
		RESULT|=AT24Cxx_IIC_Write((unsigned char)(ADDR));			/*写低地址*/
		
//中容量器件	
#elif AT24Cxx_TYPE>AT24C02			
		#define AT24Cxx_WriteADDr(ADDR,RESULT)																		\
		RESULT=AT24Cxx_IIC_Write(AT24Cxx_CMD_WRITE+((uint8_t)((ADDR>>8)&0x07)<<1));	\
		RESULT|=AT24Cxx_IIC_Write((unsigned char)(ADDR));		/*写低地址*/
		
//低容量器件
#else														
		#define AT24Cxx_WriteADDr(ADDR,RESULT)																\
		RESULT=AT24Cxx_IIC_Write(AT24Cxx_CMD_WRITE);												\
		RESULT|=AT24Cxx_IIC_Write((unsigned char)(ADDR));		/*写低地址*/
		
#endif


/*函数定义------------------------------------------------------------------------------------*/

//初始化函数 返回结果
IIC_ResultEnum AT24Cxx_Init(void)
{
	IIC_ResultEnum result;
	//初始化底层IIC驱动
	AT24Cxx_IIC_Init();

	//检查器件是否响应
	result = AT24Cxx_IIC_Start();	/* 起始标志 */

	if (IIC_ERROR_NOERROR != result) 
	{AT24Cxx_IIC_Stop();return result;}
	//总线空闲

	//读取地址0数据 检验ACK位
	result = AT24Cxx_IIC_Write(AT24Cxx_ADDRESS);	/* 写入地址 */

	AT24Cxx_IIC_Stop();				/* 停止总线 */
	return result;
}

//写入一个字节 地址高字节为器件硬件地址或页地址 低字节为字节地址
IIC_ResultEnum AT24Cxx_WriteByte(uint16_t addr,uint8_t byteData)
{
	IIC_ResultEnum result=IIC_ERROR_NOERROR;
	
	result = AT24Cxx_IIC_Start();	/*起始标志*/
	if (IIC_ERROR_NOERROR != result) 
	{AT24Cxx_IIC_Stop();return result;}
	
	AT24Cxx_WriteADDr(addr,result);	/*写入地址并读取应答结果*/
	AT24Cxx_IIC_Write(byteData);		/*写入数据*/
	AT24Cxx_IIC_Stop();							/*产生停止标志*/

	return result;
}

//应答查询
IIC_ResultEnum AT24Cxx_QueryWriteBusy(void)
{
	IIC_ResultEnum result=IIC_ERROR_NOERROR;

	result = AT24Cxx_IIC_Start();	/* 起始标志 */
	if (IIC_ERROR_NOERROR != result) 
	{AT24Cxx_IIC_Stop();return result;}

	result = AT24Cxx_IIC_Write(AT24Cxx_ADDRESS);	/* 写入地址 */

	AT24Cxx_IIC_Stop();
	return result;
}

//读取数据
uint8_t AT24Cxx_ReadByte(uint16_t addr)
{
	IIC_ResultEnum result=IIC_ERROR_NOERROR;
	unsigned char temp;
	
	result = AT24Cxx_IIC_Start();	/* 起始标志 */
	if (IIC_ERROR_NOERROR != result) 
	{AT24Cxx_IIC_Stop();return 0;}
	
	AT24Cxx_WriteADDr(addr,temp);	/* 写入地址并读取应答结果 */
	AT24Cxx_IIC_Start();					/* 起始标志 */
	AT24Cxx_IIC_Write(AT24Cxx_CMD_READ);	/* 读操作 */
	temp = AT24Cxx_IIC_Read(IIC_NOACK);		/* 读取数据 无应答 */
	AT24Cxx_IIC_Stop();						/* 停止标志 */
	return temp;
}

//写入页数据
IIC_ResultEnum AT24Cxx_WritePtr(uint16_t addr,const uint8_t* ptr,uint8_t len)
{
	IIC_ResultEnum result=IIC_ERROR_NOERROR;
	
	result = AT24Cxx_IIC_Start();	/* 起始标志 */
	if (IIC_ERROR_NOERROR != result) 
	{AT24Cxx_IIC_Stop();return result;}
	
	AT24Cxx_WriteADDr(addr,result); /* 写入地址并读取应答结果 */
	//写入数据
	while (len--)
	{
		AT24Cxx_IIC_Write(*ptr);	/* 写入数据 */
		++ptr;										/* 指向下个数据 */
	}
	AT24Cxx_IIC_Stop();					/* 停止标志 使能写入 */
	return result;
}
//读取数据到缓冲区
IIC_ResultEnum AT24Cxx_ReadPtr(uint16_t addr,uint8_t* ptr,uint8_t len)
{
	IIC_ResultEnum result=IIC_ERROR_NOERROR;
	
	if (!len) {return IIC_ERROR_NOERROR;}		/* 参数错误直接退出 */
	
	result = AT24Cxx_IIC_Start();						/* 起始标志 */
	if (IIC_ERROR_NOERROR != result) 
	{AT24Cxx_IIC_Stop();return result;}
	
	AT24Cxx_WriteADDr(addr,result);				/* 写入地址并读取应答结果 */
	result |= AT24Cxx_IIC_Start();				/* 起始标志 */
	result |= AT24Cxx_IIC_Write(AT24Cxx_CMD_READ);	/* 读操作 */

	while (--len)
	{
		*ptr = AT24Cxx_IIC_Read(IIC_ACK);	/* 读取一个字节并ACK */
		++ptr;														/* 指向下一个位置 */
	}
	*ptr = AT24Cxx_IIC_Read(IIC_NOACK);	/* 最后一个字节无ACK */

	AT24Cxx_IIC_Stop();									/* 停止标志 使能写入 */
	return result;
}

