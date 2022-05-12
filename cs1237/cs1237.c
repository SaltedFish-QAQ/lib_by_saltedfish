#include "cs1237.h"
#include "stm32f10x.h"

static void cs1237_delay(uint8_t us);
static void OUT_Config(void);
static void IN_Config(void);
static void CLK_0(void);
static void CLK_1(void);
static void DIO_0(void);
static void DIO_1(void);
static uint8_t DIO_Read(void);
static void CS1237_WriteConfigData(uint8_t txData);
static uint8_t CS1237_ReadConfigData(void);
static uint32_t CS1237_ReceiveData(void);

static void cs1237_delay(uint8_t us)
{
    for (int i = 0; i < us; i++)
    {
        for (int i = 0; i < 72; i++);
    }
}

static void OUT_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
    GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);

    GPIO_StructInit(&GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;

    GPIO_Init(GPIOB, &GPIO_InitStructure);
}

static void IN_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
    GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);

    GPIO_StructInit(&GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;

    GPIO_Init(GPIOB, &GPIO_InitStructure);
}

static void CLK_0(void)
{
    GPIO_ResetBits(GPIOB, GPIO_Pin_3);
}

static void CLK_1(void)
{
    GPIO_SetBits(GPIOB, GPIO_Pin_3);
}

static void DIO_0(void)
{
    GPIO_ResetBits(GPIOB, GPIO_Pin_4);
}

static void DIO_1(void)
{
    GPIO_SetBits(GPIOB, GPIO_Pin_4);
}

static uint8_t DIO_Read(void)
{
    return GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_4);
}

static void CS1237_WriteConfigData(uint8_t txData)
{
    uint8_t _dat = 0x80;

    CLK_0();
    IN_Config();
    while (DIO_Read() != 0x00)
    {
        
    }

    // CS1237_ReceiveData();

    for (int i = 0; i < 29; i++)
    {
        CLK_1();
        cs1237_delay(5);
        CLK_0();
        cs1237_delay(5);
    }

    OUT_Config();

    //0x56 0101 0110
    // CLK_1();cs1237_delay(5);  DIO_0();  CLK_0();cs1237_delay(5);    //30
    CLK_1();cs1237_delay(5);  DIO_1();  CLK_0();cs1237_delay(5);    //30
    CLK_1();cs1237_delay(5);  DIO_1();  CLK_0();cs1237_delay(5);    //31
    CLK_1();cs1237_delay(5);  DIO_0();  CLK_0();cs1237_delay(5);    //32
    CLK_1();cs1237_delay(5);  DIO_0();  CLK_0();cs1237_delay(5);    //33
    CLK_1();cs1237_delay(5);  DIO_1();  CLK_0();cs1237_delay(5);    //34
    CLK_1();cs1237_delay(5);  DIO_0();  CLK_0();cs1237_delay(5);    //35
    CLK_1();cs1237_delay(5);  DIO_1();  CLK_0();cs1237_delay(5);    //36

    IN_Config();
    CLK_1();cs1237_delay(5);CLK_0();cs1237_delay(5);    //37

    OUT_Config();

    for (int i = 0; i < 8; i++)
    {
        CLK_1();
        cs1237_delay(5);
        if ((txData & _dat) != 0x00)
        {
            DIO_1();
        }
        else
        {
            DIO_0();
        }
        CLK_0();
        cs1237_delay(5);
        _dat >>= 1;
    }

    CLK_1();cs1237_delay(5);CLK_0();cs1237_delay(5);    //46
}

static uint8_t CS1237_ReadConfigData(void)
{
    uint8_t configData = 0;
    CLK_0();
    IN_Config();
    while (DIO_Read() != 0x00)
    {
        
    }

    // CS1237_ReceiveData();

    for (int i = 0; i < 29; i++)
    {
        CLK_1();
        cs1237_delay(5);
        CLK_0();
        cs1237_delay(5);
    }

    OUT_Config();

    //0x56 0101 0110
    // CLK_1();cs1237_delay(5);  DIO_0();  CLK_0();cs1237_delay(5);    //30
    CLK_1();cs1237_delay(5);  DIO_1();  CLK_0();cs1237_delay(5);    //30
    CLK_1();cs1237_delay(5);  DIO_0();  CLK_0();cs1237_delay(5);    //31
    CLK_1();cs1237_delay(5);  DIO_1();  CLK_0();cs1237_delay(5);    //32
    CLK_1();cs1237_delay(5);  DIO_0();  CLK_0();cs1237_delay(5);    //33
    CLK_1();cs1237_delay(5);  DIO_1();  CLK_0();cs1237_delay(5);    //34
    CLK_1();cs1237_delay(5);  DIO_1();  CLK_0();cs1237_delay(5);    //35
    CLK_1();cs1237_delay(5);  DIO_0();  CLK_0();cs1237_delay(5);    //36

    IN_Config();
    CLK_1();cs1237_delay(5);CLK_0();cs1237_delay(5);    //37

    for (int i = 0; i < 8; i++)
    {
        CLK_1();
        cs1237_delay(5);
        configData <<= 1;
        if (DIO_Read() != 0x00)
        {
            configData++;
        }
        
        CLK_0();

        cs1237_delay(5);
    }
    
    CLK_1();cs1237_delay(5);CLK_0();cs1237_delay(5);    //46

    return configData;
}

static uint32_t CS1237_ReceiveData(void)
{
    uint32_t tempValue = 0;
    
    CLK_0();
    IN_Config();
    while (DIO_Read() != 0x00)
    {
        
    }
    
    for (int i = 0; i < 24; i++)
    {
        CLK_1();
        cs1237_delay(5);
        tempValue <<= 1;
        if (DIO_Read() != 0x00)
        {
            tempValue++;
        }
        CLK_0();
        cs1237_delay(5);
    }
    
    for (int i = 0; i < 3; i++)
    {
        CLK_1();
        cs1237_delay(5);
        // DIO_1();
        CLK_0();
        cs1237_delay(5);
    }
    
    OUT_Config();
    DIO_1();
    IN_Config();

    return tempValue;
}

uint8_t CS1237_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
    GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);

    GPIO_StructInit(&GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    return 0;
}

uint8_t CS1237_GetValue(uint16_t* currentValuePtr);
uint8_t CS1237_Test(void)
{
    CS1237_WriteConfigData(0x0c);
    CS1237_ReadConfigData();
	CS1237_ReceiveData();

    return 0;
}
