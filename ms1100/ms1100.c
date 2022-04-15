#include "ms1100.h"
#include "stm32f10x.h"

void delay_us(unsigned long time)
{
    for (int i = 0; i < time; i++)
    {
        for (int i = 0; i < 70; i++)     //1us
        {
            __NOP();
        }
    }
}

static uint8_t IIC_WriteBuffer[2] = {0};
static uint8_t IIC_ReadBuffer[4] = {0};

void MS1100A0_Write(uint8_t reg)
{
    IIC_WriteBuffer[0] = 0x90;
    IIC_WriteBuffer[1] = reg;
    IIC_Start();
    IIC_WriteBuff(&IIC_WriteBuffer[0], 2);
    IIC_Stop();
    delay_us(5000);    //5ms
}

uint16_t MS1100A0_ReadByte(void)
{
    uint8_t dataH = 0, dataL = 0, configReg = 0;
    uint16_t temp = 0;

    IIC_WriteBuffer[0] = 0x91;
    IIC_Start();
    IIC_WriteBuff(&IIC_WriteBuffer[0], 1);
    IIC_ReadBuff(&IIC_ReadBuffer[0], 3);
    IIC_Stop();

    dataH = IIC_ReadBuffer[0];
    dataL = IIC_ReadBuffer[1];
    configReg = IIC_ReadBuffer[2];

    if (configReg & 0x80)
    {
        return 0;
    }
    
    temp = dataH << 8 | dataL;

    return temp;
}

