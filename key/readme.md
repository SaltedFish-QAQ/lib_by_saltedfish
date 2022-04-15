# 使用示例

## 初始化

```C
KeyObject gStartKeyObj;
KeyObject_Init(&gStartKeyObj, gStartPort);//gstartport 在key.c中
```

## 使用示例

```C
if (gStartKeyObj.state == fallingedge)
{
	//io口上升沿事件处理
}
```

