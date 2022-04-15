#include <stdint.h>
#include <assert.h>
#include <string.h>
#include "n32g030.h"
#include "W25Qxx.h"
#include "spi_dma.h"

// SPI硬件访问对齐
#define _SPI_HW_ALIGN    (4UL)

void(*W25Q_HW_Init)(void);
uint32_t (*W25Q_HW_WriteRead)(uint8_t);
void (*W25Q_HW_Select)(void);
void (*W25Q_HW_NoSelect)(void);
void (*W25Q_HW_WriteBuff)(const void*, int);
void (*W25Q_HW_ReadBuff)(void*, int);

// 底层初始化
static void W25Q_FlashOnBoard_Init(void)
{
    RCC_EnableAPB2PeriphClk(RCC_APB2_PERIPH_GPIOA | RCC_APB2_PERIPH_GPIOB, ENABLE);
    RCC_EnableAPB2PeriphClk(RCC_APB2_PERIPH_SPI2 | RCC_APB2_PERIPH_AFIO, ENABLE);

    GPIO_InitType GPIO_InitStructure;
    GPIO_InitStruct(&GPIO_InitStructure);

    GPIO_InitStructure.Pin = SPI_NCSS1_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitPeripheral(SPI_NCSS1_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.Pin = SPI_NCSS2_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitPeripheral(SPI_NCSS2_PORT, &GPIO_InitStructure);

    GPIO_InitStruct(&GPIO_InitStructure);

    GPIO_InitStructure.Pin = SPI_SCLK_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_MODE_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_SPEED_HIGH;
    GPIO_InitStructure.GPIO_Alternate = GPIO_AF0_SPI2;
    GPIO_InitPeripheral(SPI_SCLK_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.Pin = SPI_MISO_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_MODE_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_SPEED_HIGH;
    GPIO_InitStructure.GPIO_Alternate = GPIO_AF0_SPI2;
    GPIO_InitPeripheral(SPI_MISO_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.Pin = SPI_MOSI_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_MODE_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_SPEED_HIGH;
    GPIO_InitStructure.GPIO_Alternate = GPIO_AF8_SPI2;
    GPIO_InitPeripheral(SPI_MOSI_PORT, &GPIO_InitStructure);

    SPI_InitType SPI_InitStructure;
    SPI_InitStruct(&SPI_InitStructure);

    //配置SPI2参数
    SPI_InitStructure.SpiMode       = SPI_MODE_MASTER;                  //模式为主模式
    SPI_InitStructure.DataLen       = SPI_DATA_SIZE_8BITS;              //数据宽度8bit
    SPI_InitStructure.CLKPOL        = SPI_CLKPOL_LOW;                   
    SPI_InitStructure.CLKPHA        = SPI_CLKPHA_FIRST_EDGE;
    SPI_InitStructure.NSS           = SPI_NSS_SOFT;
    SPI_InitStructure.BaudRatePres  = SPI_BR_PRESCALER_2;
    SPI_InitStructure.FirstBit      = SPI_FB_MSB;
    SPI_Init(SPI2, &SPI_InitStructure);

    //使能SPI2
    SPI_Enable(SPI2, ENABLE);
}

// 底层读写
static uint32_t W25Q_FlashOnBoard_WriteRead(uint8_t wbyte)
{
    while (SPI_I2S_GetStatus(SPI2, SPI_I2S_TE_FLAG) == RESET);
    SPI_I2S_TransmitData(SPI2, wbyte);
    while(SPI_I2S_GetStatus(SPI2, SPI_I2S_RNE_FLAG) == RESET);
    return SPI_I2S_ReceiveData(SPI2);
}

// CS片选
static void W25Q_FlashOnBoard_Select(void)
{
    GPIO_ResetBits(SPI_NCSS1_PORT, SPI_NCSS1_PIN);
}

// CS取消片选
static void W25Q_FlashOnBoard_NoSelect(void)
{
    GPIO_SetBits(SPI_NCSS1_PORT, SPI_NCSS1_PIN);

}

// CS片选
static void W25Q_FlashOffBoard_Select(void)
{
    GPIO_ResetBits(SPI_NCSS2_PORT, SPI_NCSS2_PIN);
}

// CS取消片选
static void W25Q_FlashOffBoard_NoSelect(void)
{
    GPIO_SetBits(SPI_NCSS2_PORT, SPI_NCSS2_PIN);

}

// 写缓冲区
static void W25Q_FlashOnBoard_WriteBuff(const void* pbuff, int size)
{
    for (; size; --size, pbuff = (uint8_t*)pbuff+1)
        W25Q_HW_WriteRead(*(uint8_t*)pbuff);
}

// 批量读取到缓冲区
static void W25Q_FlashOnBoard_ReadBuff(void* pbuff, int size)
{
    uint8_t* pbuffPtr = (uint8_t*)pbuff;

    if (SPI_FLASH_DMA_STATE == SPIDMANOTUSEFLAG)
    {
        for (int i = 0; i < size; i++)
        {
            pbuffPtr[i] = W25Q_HW_WriteRead(0);
        }
    }
    else if (SPI_FLASH_DMA_STATE == SPIDMAUSEFLAG)
    {
        SPIDMA_Spi2Read(pbuff, size);
    }
}


void W25Q_SetFlashFuncPtr(uint8_t flash_index)
{
    if (flash_index == FLASH_ONBOARD)
    {
        W25Q_HW_Init = W25Q_FlashOnBoard_Init;
        W25Q_HW_WriteRead = W25Q_FlashOnBoard_WriteRead;
        W25Q_HW_Select = W25Q_FlashOnBoard_Select;
        W25Q_HW_NoSelect = W25Q_FlashOnBoard_NoSelect;
        W25Q_HW_WriteBuff = W25Q_FlashOnBoard_WriteBuff;
        W25Q_HW_ReadBuff = W25Q_FlashOnBoard_ReadBuff;
    }
    else if (flash_index == FLASH_OFFBOARD)
    {
        W25Q_HW_Init = W25Q_FlashOnBoard_Init;
        W25Q_HW_WriteRead = W25Q_FlashOnBoard_WriteRead;
        W25Q_HW_Select = W25Q_FlashOffBoard_Select;
        W25Q_HW_NoSelect = W25Q_FlashOffBoard_NoSelect;
        W25Q_HW_WriteBuff = W25Q_FlashOnBoard_WriteBuff;
        W25Q_HW_ReadBuff = W25Q_FlashOnBoard_ReadBuff;
    }
    
}

