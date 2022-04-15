/***********************************************************************************************
文件名	AT24Cxx.h
日期	2013.4.29
作者	张诗星	陈东辉
文件说明	AT24C02/08/16/32/64/256/512库头文件
修订说明:

2013.04.29  张诗星 初始版本
2013.05.30	张诗星 增加错误检测
2013.07.24	张诗星 增加AT24C256等高容量支持 去掉多器件支持特性
2013.10.12	张诗星 增加CMD_READ CMD_WRITE命令宏定义
***********************************************************************************************/

#ifndef _LIB_AT_24CXX_H_
#define _LIB_AT_24CXX_H_
/*头文件--------------------------------------------------------------------------------------*/

#include <stdint.h>
#include "sIIC.h"

/*用户配置------------------------------------------------------------------------------------*/

//器件类型定义
#define AT24Cxx_TYPE AT24C04			/*选定的器件*/

//器件地址
#define AT24Cxx_AddrID	0


/*宏定义--------------------------------------------------------------------------------------*/

//器件最大地址定义
#define AT24C01		0x7F				/* AT24C01最大地址 */
#define AT24C02		0xFF				/* AT24C02最大地址 */
#define AT24C04		0x1FF				/* AT24C04最大地址 */
#define AT24C08		0x3FF				/* AT24C08最大地址 */
#define AT24C16		0x7FF				/* AT24C16最大地址 */
#define AT24C32		0xFFF				/* AT24C32最大地址 */
#define AT24C64	    0x1FFF			    /* AT24C64最大地址 */
#define AT24C128		0x3FFF			/* AT24C128最大地址 */
#define AT24C256		0x7FFF  		/* AT24C256最大地址 */
#define AT24C512		0xFFFF  		/* AT24C512最大地址 */

//页缓冲大小定义
#define AT24C01_PAGE     (8)
#define AT24C02_PAGE     (16)
#define AT24C04_PAGE     (16)
#define AT24C08_PAGE     (16)
#define AT24C016_PAGE    (16)
#define AT24C032_PAGE    (32)
#define AT24C064_PAGE    (32)
#define AT24C0128_PAGE   (64)
#define AT24C0256_PAGE   (64)
#define AT24C0512_PAGE   (64)

/*接口函数------------------------------------------------------------------------------------*/

//初始化函数 返回结果
IIC_ResultEnum AT24Cxx_Init(void);

//写入一个字节
IIC_ResultEnum AT24Cxx_WriteByte(uint16_t addr,uint8_t byteData);

//应答查询
IIC_ResultEnum AT24Cxx_QueryWriteBusy(void);

//读取数据
uint8_t AT24Cxx_ReadByte(uint16_t addr);

//写入缓冲区数据
IIC_ResultEnum AT24Cxx_WritePtr(uint16_t addr,const uint8_t* ptr,uint8_t len);

//读取数据到缓冲区
IIC_ResultEnum AT24Cxx_ReadPtr(uint16_t addr,uint8_t* ptr,uint8_t len);

#endif


