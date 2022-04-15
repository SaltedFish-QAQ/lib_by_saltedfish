# 使用示例

## 初始化

```C
KeyObject gStartKeyObj;
KeyObject_Init(&gStartKeyObj, gStartPort);//gstartport 在key.c中
```

## 使用示例

```C
KeyScanInput(&gStartKeyObj);

if (gStartKeyObj.state == fallingedge)
{
	//io口上升沿事件处理
}
```

## 使用指南

通过对象内部的事件记录器记录发生的io事件，所有的事件获得与判断在所有平台通用。只需要做好io移植和初始化工作就可以了。

以下是移植以及具体的事件说明

### 移植

一直主要是函数ExterPort_Init内的GPIO口移植，因为这个初始化函数并未暴露给外部使用，所以基础的IO口声明需要放在key.c这个文件内部

```C
ExterPort_t gStartPort = 
{
    .port = GPIOD,		//	gpio组
    .pin = GPIO_Pin_1,	//	gpio管脚
};


static void ExterPort_Init(void);
static uint8_t ExterPort_GetInPutLevel(ExterPort_t exterPort);
static void ExterPort_SetOutPutLevel(ExterPort_t exterPort, ExterPortState state);

static void ExterPort_Init(void)
{
    //gpio初始化
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);

    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_StructInit(&GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = gStartPort.pin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
    GPIO_Init(gStartPort.port, &GPIO_InitStructure);
}

static uint8_t ExterPort_GetInPutLevel(ExterPort_t exterPort)
{
    //读电平函数
    return GPIO_ReadInputDataBit(exterPort.port, exterPort.pin);;
}

static void ExterPort_SetOutPutLevel(ExterPort_t exterPort, ExterPortState state)
{
    //写电平函数
    GPIO_WriteBit(exterPort.port, exterPort.pin, (BitAction)state);
}

```



### 事件说明

```C
//io口电平
typedef enum 
{
   lowlevel = 0,    //低电平
   highlevel,       //高电平
}ExterPortState;

//按键事件
typedef enum 
{
   fallingedge,     //下降沿事件
   risingedge,      //上升沿事件
   longpress,       //长按事件
   press,           //按下事件
   nopress,         //松开事件
}KeyState;
```

### 长按事件

长按事件是基于轮询调用次数判断的。实际实现是当按下状态保持超过设定次数的轮询次数时，上报长按事件。

长按实现

```c
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
```



### 注意事项

<u>**使用本模块需要注意。本模块是基于逻辑下的死循环式的程序设计的。所以未考虑事件队列功能。所以务必在进行一次按键扫描后立即开始事件查询处理，否则会发生按键事件丢失的情况。**</u>

