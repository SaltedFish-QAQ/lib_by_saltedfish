/// \file   W25Qxx.h
/// \brief  W25Qxx SPI Flash支持库
///
/// \author 张诗星
/// \par
/// (C) Copyright 杭州大仁科技有限公司
///
/// \version
/// 2018/4/22 张诗星 初始版本\n

#ifndef _W_25_Q_XX_H
#define _W_25_Q_XX_H

// *****************************************************************************************
// 头文件

#include <stdint.h>
#include <stdbool.h>

// *****************************************************************************************
// 用户配置

/// 定义flash容量(字节)
#define W25Q_CFG_FLASH_SIZE     (4*1024*1024)

/// 定义flash最小可编程页大小(字节)
#define W25Q_CFG_PAGE_SIZE      (256)

/// 定义flash最小可擦除扇区大小(字节)
#define W25Q_CFG_SECTOR_SIZE    (4*1024)

#define SPI_SCLK_PORT   GPIOB
#define SPI_SCLK_PIN    GPIO_PIN_0

#define SPI_NCSS1_PORT   GPIOA
#define SPI_NCSS1_PIN    GPIO_PIN_7

#define SPI_NCSS2_PORT   GPIOA
#define SPI_NCSS2_PIN    GPIO_PIN_6

#define SPI_MISO_PORT   GPIOA
#define SPI_MISO_PIN    GPIO_PIN_12

#define SPI_MOSI_PORT   GPIOB
#define SPI_MOSI_PIN    GPIO_PIN_1

#define FLASH_ONBOARD   0
#define FLASH_OFFBOARD  1

// *****************************************************************************************
// 宏定义

// 状态位定义
#define W25Q_STATUS_BUSY    (1UL<<0)    /// 忙标志只读 1正在写操作
#define W25Q_STATUS_WEL     (1UL<<1)    /// 写使能位只读 1写保护，上电、编程、擦除、写状态寄存器后自动清理
#define W25Q_STATUS_BP0     (1UL<<2)    /// 块写保护0
#define W25Q_STATUS_BP1     (1UL<<3)    /// 块写保护1
#define W25Q_STATUS_BP2     (1UL<<4)    /// 块写保护2
#define W25Q_SREG_SRP       (1UL<<7)    /// 状态寄存器写保护位

// *****************************************************************************************
// 接口定义
extern void W25Q_SetDMAUseOrNotUse(uint8_t flag);
extern void W25Q_SetFlashFuncPtr(uint8_t flash_index);
void W25Q_Init(void);
uint8_t W25Q_ReadStatus(void);
void W25Q_WriteStatus(uint8_t statusData);
void W25Q_ReadUniqueID(uint8_t pbuff[]);
uint32_t W25Q_ReadJEDECID(void);
void W25Q_FastReadBuff(uint32_t readAddr, void* pBuffer, uint32_t numByteToRead);
void W25Q_EraseChip(void);
void W25Q_ErasePage(uint32_t address);
void W25Q_PowerDown(void);
void W25Q_ReleasePowerDown(void);
void W25Q_PageProgram(uint32_t address, const void* pBuffer, uint32_t numByteToProgram);
bool W25Q_DeviceIsBusy(void);
bool W25Q_WriteEnableLatch(void);

#endif

