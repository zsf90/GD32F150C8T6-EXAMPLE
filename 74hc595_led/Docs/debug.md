# gd32 串口中断收发数据



要实现的功能：

1. 以中断的方式接收数据
2. 以中断的方式发送数据

## 中断发送数据

用按键控制串口发送数据，用户按一下按钮，单片机发送一个数据。这里需要用到外部中断。

## 用串口助手发送数据，单片机接收数据

使用串口助手给单片机发送数据

# 2021-11-05 增加 SPI 驱动数码管

调试当中出现的错误

1. 用逻辑分析仪测试波形不对，原因是数码管的VCC电压接到了5V,数码管的VCC、SDI、LOAD的电平应该统一。
2. 端子接触不良，浪费了花很多时间

# 2021-11-7 定时器调试

1. 发现核心板8M晶振有问题 换成内部无问题


# 2021-11-7 滴答定时器按键消抖

