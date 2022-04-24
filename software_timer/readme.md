## 原理解释

利用定时器基本功能，通过数组实时记录上一次访问本模块时的计数值。通过刚刚获得的计数值进行减运算，获得两次访问之间的间隔时间（conters * 1 / psc）。目前支持五个软件定时器，可扩展数组进行扩大。模块中使用宏定义定义每一个软件定时器的id号，也可以使用别的方式。

需要注意的是在使用os时，本模块的精度会相比于死循环协作式程序更加准确。但是在使用os时，os会提供默认的软件定时器功能，就显得很鸡肋了。所以一下声明主要针对死循环协作式程序。在协作式程序中，该模块的精度除了取决于定时器，更大程度上取决于程序负载。即在两次访问间隔中的代码运行速度。例如：假设需要判断的访问间隔为1000ms，但是两次访问之间的代码需要的执行时间为1500ms。则通常情况下，该间隔为1500ms而非本模块给出的1000ms。本模块实际上只会判断上次访问到本次访问时中间的时间间隔有没有超过1000ms，并不会在到达1000ms时立即打断代码运行开始派发事件。所以进量减少两次访问之间的代码量。以及在协作式程序中进量使用状态机，多分状态的形式，进量少使用while（）式的等待执行方式。否则会出现精度的严重不确定性。

使用无符号的数据进行减运算，再次转换为无符号数据时并不会出现结果错误的情况。0 - 0xff = 0xff，该部分原理参考c语言中的整数与负数部分。不再赘述。

## 移植工作

移植工作主要为初始化函数和定时器获得计数值的部分，示例如下：

```C
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

```

在本模块中使用函数指针的形式进行调用

```C
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
```



## 使用示例

```C
SoftwareTimer_GetCurrentMs(CONTORLTIMER, &ms);	//通过指定ID获得其对应的距离上一次调用该函数（访问本模块）过去的时间间隔
if (ms >= 20000)							 //当该时间大于2000ms时（因为mcu差异，定时器模块只能分频到100us一个计数）
{
    doSomething();
    oftwareTimer_UpdatePreviousMS(CONTORLTIMER);//重新更新对应ID内的暂存计数值，用于下一次访问模块时计算时间间隔
    SoftwareTimer_ClrCurrentMS(CONTORLTIMER);	//清楚存储的时间间隔数值
}
```

