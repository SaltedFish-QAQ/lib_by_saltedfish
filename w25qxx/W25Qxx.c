/// \file   W25Qxx.c
/// \brief  W25Qxx SPI Flash支持库
///
/// \author 张诗星
/// \par
/// (C) Copyright 杭州大仁科技有限公司
///
/// \version
/// 2018/4/22 张诗星 初始版本\n


// *****************************************************************************************
// 头文件
#include <assert.h>
#include "W25Qxx.h"
// #include "W25Qxx_HW.c"

// *****************************************************************************************
// 内部定义

// 操作命令定义
enum
{
    CMD_WRITE_ENABLE    = 0x06, /* 写使能 */
    CMD_WRITE_DISABLE   = 0x04, /* 写禁用 */
    CMD_READ_STATUS     = 0x05, /* 读取状态寄存器 */
    CMD_WRITE_STATUS    = 0x01, /* 写状态寄存器 */
    CMD_READ_DATA       = 0x03, /* 读取数据 */
    CMD_FAST_READ_DATA  = 0x0b, /* 快速读取 允许最高频率读取 */
    CMD_PAGE_PROGRAM    = 0x02, /* 页编程 */
    CMD_ERASE_64KBLOCK  = 0xd8, /* 块擦除 64K */
    CMD_ERASE_32KBLOCK  = 0x52, /* 块擦除 32K */
    CMD_SECTOR_ERASE    = 0x20, /* 扇区擦除 */
    CMD_CHIP_ERASE      = 0xc7, /* 整片擦除 */

    // 电源管理命令
    CMD_POWER_DOWN      = 0xb9, /* 待机 */
    CMD_RELEASE_POWER_DOWN      = 0xab, /* 取消待机 */

    // ID命令
    CMD_READ_UID        = 0x4b, /* 读取唯一ID号 */
    CMD_READ_JEDEC_ID   = 0x9f, /* 读取器件ID */
};

extern void(*W25Q_HW_Init)(void);
extern uint32_t (*W25Q_HW_WriteRead)(uint8_t);
extern void (*W25Q_HW_Select)(void);
extern void (*W25Q_HW_NoSelect)(void);
extern void (*W25Q_HW_WriteBuff)(const void*, int);
extern void (*W25Q_HW_ReadBuff)(void*, int);

// 写使能
static void W25Q_WriteEnable(void)
{
    W25Q_HW_Select();
    W25Q_HW_WriteRead(CMD_WRITE_ENABLE);
    W25Q_HW_NoSelect();
}

// 写禁用
static void W25Q_WriteDisable(void)
{
    W25Q_HW_Select();
    W25Q_HW_WriteRead(CMD_WRITE_DISABLE);
    W25Q_HW_NoSelect();
}


// *****************************************************************************************
// 接口定义

/// \brief 初始化W25Qxx芯片
///
/// \return void
void W25Q_Init(void)
{
    W25Q_HW_Init();
    W25Q_HW_NoSelect();
}

/// \brief 读取状态寄存器
///
/// \return uint8_t 状态寄存器结果
uint8_t W25Q_ReadStatus(void)
{
    uint32_t temp = 0;
    W25Q_HW_Select();

    W25Q_HW_WriteRead(CMD_READ_STATUS);     /* 读状态寄存器命令 */
    temp = W25Q_HW_WriteRead(0);

    W25Q_HW_NoSelect();
    return temp;
}

/// \brief 写状态寄存器
///
/// \param statusData 写入状态寄存器值
/// \return void
void W25Q_WriteStatus(uint8_t statusData)
{
    // 写使能,操作完成后自动写禁用
    W25Q_WriteEnable();

    W25Q_HW_Select();

    W25Q_HW_WriteRead(CMD_WRITE_STATUS);     /* 读状态寄存器命令 */
    W25Q_HW_WriteRead(statusData);

    W25Q_HW_NoSelect();
}

/// \brief 读取芯片8字节的唯一序列号
///
/// \param pbuff 保存UID的数组
/// \return void
void W25Q_ReadUniqueID(uint8_t pbuff[])
{
    W25Q_HW_Select();

    // 发送读UID命令和4字节占位数据
    W25Q_HW_WriteRead(CMD_READ_UID);
    W25Q_HW_WriteRead(0);
    W25Q_HW_WriteRead(0);
    W25Q_HW_WriteRead(0);
    W25Q_HW_WriteRead(0);

    // 读取结果
    for (int i = 0; i < 8; ++i)
        pbuff[i] = W25Q_HW_WriteRead(0);

    W25Q_HW_NoSelect();
}

/// \brief 读取3字节JEDEC 器件ID
///
/// \return uint32_t 读取结果
uint32_t W25Q_ReadJEDECID(void)
{
    uint32_t temp = 0;
    W25Q_HW_Select();

    // 发送读ID命令
    W25Q_HW_WriteRead(CMD_READ_JEDEC_ID);

    // 读取结果
    temp |= (uint32_t)W25Q_HW_WriteRead(0)<<24;
    temp |= (uint32_t)W25Q_HW_WriteRead(0)<<16;
    temp |= (uint32_t)W25Q_HW_WriteRead(0)<<8;

    W25Q_HW_NoSelect();

    return temp;
}

/// \brief 快速读取多个数据
///
/// \details 使用快速读取命令读取，可允许更高的时钟频率
///
/// \param readAddr 数据地址
/// \param pBuffer 保存读取数据内存指针
/// \param numByteToRead 读取字节数
/// \return void
void W25Q_FastReadBuff(uint32_t readAddr, void* pBuffer, uint32_t numByteToRead)
{
    W25Q_HW_Select();

    // 写入命令地址
    W25Q_HW_WriteRead(CMD_FAST_READ_DATA);      /* 使用快速读取 */
    W25Q_HW_WriteRead((uint8_t)(readAddr>>16));
    W25Q_HW_WriteRead((uint8_t)(readAddr>>8));
    W25Q_HW_WriteRead((uint8_t)readAddr);
    W25Q_HW_WriteRead(0);       /* 额外的8位占位时钟，以使芯片有额外的时间设置地址 */

    // 读取数据
    W25Q_HW_ReadBuff(pBuffer, numByteToRead);
    
    W25Q_HW_NoSelect();
}

/// \brief 全片擦除
///
/// \return void
void W25Q_EraseChip(void)
{
    // 写使能,操作完成后自动写禁用
    W25Q_WriteEnable();

    // 片选器件
    W25Q_HW_Select();

    W25Q_HW_WriteRead(CMD_CHIP_ERASE);

    // 取消片选,开始擦除
    W25Q_HW_NoSelect();
}

/// \brief 擦除一页数据
///
/// \param address 擦除地址
/// \return void
void W25Q_ErasePage(uint32_t address)
{
    // 写使能,操作完成后自动写禁用
    W25Q_WriteEnable();

    // 片选器件
    W25Q_HW_Select();

    // 写擦除命令
    W25Q_HW_WriteRead(CMD_SECTOR_ERASE);

    // 擦除地址
    W25Q_HW_WriteRead((uint8_t)(address>>16));
    W25Q_HW_WriteRead((uint8_t)(address>>8));
    W25Q_HW_WriteRead((uint8_t)address);

    // 取消片选,开始擦除
    W25Q_HW_NoSelect();
}

/// \brief 使器件进入低功耗状态
///
/// \return void
void W25Q_PowerDown(void)
{
    W25Q_HW_Select();
    W25Q_HW_WriteRead(CMD_POWER_DOWN);
    W25Q_HW_NoSelect();
}

/// \brief 使器件退出低功耗状态
///
/// \return void
void W25Q_ReleasePowerDown(void)
{
    W25Q_HW_Select();
    W25Q_HW_WriteRead(CMD_RELEASE_POWER_DOWN);
    W25Q_HW_NoSelect();
}

/// \brief 写入并编程1页数据
///
/// \param address 写入地址，必须页对齐
/// \param pBuffer 待写入的一页数据
/// \param numByteToProgram 编程数据字节数
/// \return void
void W25Q_PageProgram(uint32_t address, const void* pBuffer, uint32_t numByteToProgram)
{
    assert(numByteToProgram <= W25Q_CFG_PAGE_SIZE);
    
    if (numByteToProgram > W25Q_CFG_PAGE_SIZE)
        numByteToProgram = W25Q_CFG_PAGE_SIZE;
    
    // 写使能,操作完成后自动写禁用
    W25Q_WriteEnable();
    
    // 片选器件
    W25Q_HW_Select();

    // 写入命令、地址、数据
    W25Q_HW_WriteRead(CMD_PAGE_PROGRAM);
    W25Q_HW_WriteRead((uint8_t)(address>>16));
    W25Q_HW_WriteRead((uint8_t)(address>>8));
    W25Q_HW_WriteRead((uint8_t)address);

    W25Q_HW_WriteBuff(pBuffer, numByteToProgram);

    // 取消片选,开始编程
    W25Q_HW_NoSelect();
}

/// \brief 检查器件是否处于忙状态
///
/// \return bool true器件在忙(擦除\写入)
bool W25Q_DeviceIsBusy(void)
{
    return W25Q_ReadStatus()&W25Q_STATUS_BUSY;
}

/// \brief 检查器件是否处于写使能状态
///
/// \return bool true器件写使能
bool W25Q_WriteEnableLatch(void)
{
    return W25Q_ReadStatus()&W25Q_STATUS_WEL;
}
