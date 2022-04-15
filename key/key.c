#include "key.h"

#define KEYLONGPRESSTIME       1300     //长按键延时
#define SAMPLINGTIMES           1      //采样次数
#define MINQUALIFIEDTIMES       1      //采样合格次数

ExterPort_t gStartPort = 
{
    .port = GPIOD,
    .pin = GPIO_Pin_1,
};

//***************************************************静态函数****************************************************
static void ExterPort_Init(void);
static uint8_t ExterPort_GetInPutLevel(ExterPort_t exterPort);
static void ExterPort_SetOutPutLevel(ExterPort_t exterPort, ExterPortState state);

static void ExterPort_Init(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);

    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_StructInit(&GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = gStartPort.pin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
    GPIO_Init(gStartPort.port, &GPIO_InitStructure);
}

static uint8_t ExterPort_GetInPutLevel(ExterPort_t exterPort)
{
    return GPIO_ReadInputDataBit(exterPort.port, exterPort.pin);;
}

static void ExterPort_SetOutPutLevel(ExterPort_t exterPort, ExterPortState state)
{
    GPIO_WriteBit(exterPort.port, exterPort.pin, (BitAction)state);
}

static ExterPortState ExterPortObject_GetState(ExterPortObject* pobj);

/// \brief        ExterPortObject_GetState
/// \details      根据传入的端口对象，滤波采样其当前电平
///                
/// \param        pobj
/// \param        
/// \return       ExterPortState
static ExterPortState ExterPortObject_GetState(ExterPortObject* pobj)
{
    ExterPortState state = highlevel;
    uint8_t times = 0;

    for (int i = 0; i < SAMPLINGTIMES; i++)
    {
        if (!pobj->exterPort_Check(pobj->exterPort))
        {
            times++;
        }
    }

    if (times >= MINQUALIFIEDTIMES)
    {
        state = lowlevel;
    }
    else
    {
        state = highlevel;
    }
    
    return state;
}

//***************************************************接口函数****************************************************
void ExterPort_HWInit(void)
{
    ExterPort_Init();
}
/// \brief        ExterPortObject_Init
/// \details      初始化接口对象
///                
/// \param        pobj
/// \param        port
/// \return       void
void ExterPortObject_Init(ExterPortObject* pobj, ExterPort_t port)
{
    ExterPort_HWInit();
    pobj->exterPort = port;
    pobj->exterPort_Check = ExterPort_GetInPutLevel;
    pobj->exterPort_outPut = ExterPort_SetOutPutLevel;
    pobj->state = highlevel;
}

/// \brief        KeyObject_Init
/// \details      初始化按键对象
///                
/// \param        pobj
/// \param        port
/// \return       void
void KeyObject_Init(KeyObject* pobj, ExterPort_t port)
{
    ExterPortObject_Init(&(pobj->exterPortObject), port);
    pobj->state = nopress;
    pobj->pressTimer = 0;
    pobj->longPressDelay = KEYLONGPRESSTIME;
}

/// \brief        KeyScanInput
/// \details      按键扫描函数，于循环中周期调用
///                
/// \param        pobj
/// \param        
/// \return       void
void KeyScanInput(KeyObject* pobj)
{
    ExterPortState currentPortLevel = highlevel;

    currentPortLevel = ExterPortObject_GetState(&(pobj->exterPortObject));

    if (currentPortLevel != pobj->exterPortObject.state)
    {
        if (pobj->exterPortObject.state == highlevel)
        {
            pobj->state = fallingedge;
        }
        else if (pobj->exterPortObject.state == lowlevel)
        {
            pobj->state = risingedge;
        }
        pobj->exterPortObject.state = currentPortLevel;
        return;
    }

    if (pobj->exterPortObject.state == lowlevel)
    {
        pobj->state = press;
        pobj->pressTimer++;
    }
    else if (pobj->exterPortObject.state == highlevel)
    {
        pobj->state = nopress;
        pobj->pressTimer = 0;
    }
    
    
    if (pobj->pressTimer >= pobj->longPressDelay)
    {
        pobj->state = longpress;
        pobj->pressTimer = 0;
    }
    
}
