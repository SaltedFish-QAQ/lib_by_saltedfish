#include "software_timer.h"
#include "stm32f10x.h"

void (*timerInit)(void);
uint32_t (*timerGetValue)(void);

static software_timer_t timerArray[5] = {0};

static void Timer_Init(void);
static uint32_t Timer_GetValue(void);

static void Timer_Init(void)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

    //定时器 TIM3 初始化
    TIM_TimeBaseStructure.TIM_Period = 0xffff; //设置自动重装载寄存器周期的值
    TIM_TimeBaseStructure.TIM_Prescaler =7199; //设置时钟频率除数的预分频值, 计数值为1时为100us，0.1ms
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //TIM 向上计数
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //②初始化 TIM3

    TIM_Cmd(TIM3, ENABLE); //⑤使能 TIM3
}

static uint32_t Timer_GetValue(void)
{
    return (TIM_GetCounter(TIM3));//计数值为1时为100us
}

uint8_t SoftwareTimer_Init(void)
{
    timerInit = Timer_Init;
    timerGetValue = Timer_GetValue;

    if (timerInit != NULL)
    {
        timerInit();

        return 0;
    }
    
    return 1;
}

uint8_t SoftwareTimer_Register(uint8_t ID)
{
    for (int i = 0; i < 5; i++)
    {
        if (timerArray[i].timerUseFlag == 0)
        {
            timerArray[i].timerID = ID;
            timerArray[i].timerUseFlag = 1;

            return 0;
        }
    }
    
    return 1;
}

uint8_t SoftwareTimer_GetCurrentMs(uint8_t ID, uint16_t* ms)
{
    uint16_t tempTime = 0;
    uint8_t index = 0;

    if (timerGetValue == NULL)
    {
        return 1;
    }
    
    for (int i = 0; i < 5; i++)
    {
        if (timerArray[i].timerID == ID)
        {
            index = i;
            break;
        }
    }
    

    tempTime = timerGetValue();
    timerArray[index].timerCurrentMS = tempTime - timerArray[ID].timerPreviousMS;
    *ms = timerArray[index].timerCurrentMS;

    return 0;
}

uint8_t SoftwareTimer_UpdatePreviousMS(uint8_t ID)
{
    uint8_t index = 0;

    if (timerGetValue == NULL)
    {
        return 1;
    }

    for (int i = 0; i < 5; i++)
    {
        if (timerArray[i].timerID == ID)
        {
            index = i;
            break;
        }
    }

    timerArray[index].timerPreviousMS = timerGetValue();

    return 0;
}

uint8_t SoftwareTimer_ClrCurrentMS(uint8_t ID)
{
    uint8_t index = 0;

    for (int i = 0; i < 5; i++)
    {
        if (timerArray[i].timerID == ID)
        {
            index = i;
            break;
        }
    }

    timerArray[index].timerCurrentMS = 0;

    return 0;
}
