/***********************************************************************************************
�ļ���	AT24Cxx.h
����	2013.4.29
����	��ʫ��	�¶���
�ļ�˵��	AT24C02/08/16/32/64/256/512��ͷ�ļ�
�޶�˵��:

2013.04.29  ��ʫ�� ��ʼ�汾
2013.05.30	��ʫ�� ���Ӵ�����
2013.07.24	��ʫ�� ����AT24C256�ȸ�����֧�� ȥ��������֧������
2013.10.12	��ʫ�� ����CMD_READ CMD_WRITE����궨��
***********************************************************************************************/

#ifndef _LIB_AT_24CXX_H_
#define _LIB_AT_24CXX_H_
/*ͷ�ļ�--------------------------------------------------------------------------------------*/

#include <stdint.h>
#include "sIIC.h"

/*�û�����------------------------------------------------------------------------------------*/

//�������Ͷ���
#define AT24Cxx_TYPE AT24C04			/*ѡ��������*/

//������ַ
#define AT24Cxx_AddrID	0


/*�궨��--------------------------------------------------------------------------------------*/

//��������ַ����
#define AT24C01		0x7F				/* AT24C01����ַ */
#define AT24C02		0xFF				/* AT24C02����ַ */
#define AT24C04		0x1FF				/* AT24C04����ַ */
#define AT24C08		0x3FF				/* AT24C08����ַ */
#define AT24C16		0x7FF				/* AT24C16����ַ */
#define AT24C32		0xFFF				/* AT24C32����ַ */
#define AT24C64	    0x1FFF			    /* AT24C64����ַ */
#define AT24C128		0x3FFF			/* AT24C128����ַ */
#define AT24C256		0x7FFF  		/* AT24C256����ַ */
#define AT24C512		0xFFFF  		/* AT24C512����ַ */

//ҳ�����С����
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

/*�ӿں���------------------------------------------------------------------------------------*/

//��ʼ������ ���ؽ��
IIC_ResultEnum AT24Cxx_Init(void);

//д��һ���ֽ�
IIC_ResultEnum AT24Cxx_WriteByte(uint16_t addr,uint8_t byteData);

//Ӧ���ѯ
IIC_ResultEnum AT24Cxx_QueryWriteBusy(void);

//��ȡ����
uint8_t AT24Cxx_ReadByte(uint16_t addr);

//д�뻺��������
IIC_ResultEnum AT24Cxx_WritePtr(uint16_t addr,const uint8_t* ptr,uint8_t len);

//��ȡ���ݵ�������
IIC_ResultEnum AT24Cxx_ReadPtr(uint16_t addr,uint8_t* ptr,uint8_t len);

#endif


