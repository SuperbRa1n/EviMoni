# EviMoni
EviMoni项目组节能减排项目
## 说明
* 硬件部分采用STM32F103C8T6主控，相关代码已经放在`EviMoni_Hard`目录下
* 软件部分使用微信小程序纯自主开发，相关代码已经放在`EviMoni_Soft`目录下
* 硬件部分采用CubeMX+PlatformIO的方式开发，在CubeIDE上也同样通过了编译测试
* 目前软硬件部分代码均未检验正确性
* 目前硬件部分传感器和ESP8266尚未做MX配置的适配，其中冲突保留在了分支`zhl`中
## 传感器部分
* IO口使用情况如下图

  ![](https://cdn.jsdelivr.net/gh/SuperbRa1n/ImageLib/io.png)
* 各外设使用情况
  * GPIO: 
    * PA6——GP2Y1014AU的LED灯
    * PA4——HC-SR04的TRIG
  * ADC:
    * ADC1&DMA——光敏和CO2数据
    * ADC2——GP2Y1014AU数据
  * SPI1:
    为屏幕预留，目前暂未使用
  * I2C1:
    SGP30数据
  * TIM1:
    HC-SR01数据
  * TIM4:
    用于控制精确的微秒级计时
## ESP8266部分
* IO口使用情况如下图

  ![](https://cdn.jsdelivr.net/gh/SuperbRa1n/ImageLib/1280X1280.PNG)
* 各外设使用情况
  * PC14:esp8266使能
  * USART2:eps8266通信
  * PB0，PB1，PB2，SPI2：GC9A01屏幕驱动
  * PB3：屏幕亮度pwm
  * USART1:调试使用
  * 预计触摸屏模块CST816D还需要I2C,RST和一个上升下降沿捕获io口
* 说明
  * 目前找不到CST816D模块相关寄存器手册或现成代码，还未开发
  * tft GUI设计还在写中
  * esp8266和GC9A01驱动已完成且编译通过，前者只完成与微信端连接，还未完成数据接收提取（发送端的数据结构还没看）
