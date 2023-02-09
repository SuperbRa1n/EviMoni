# EviMoni
EviMoni项目组节能减排项目
## 说明
* 硬件部分采用STM32F103C8T6主控，相关代码已经放在`EviMoni_Hard`目录下
* 软件部分使用微信小程序纯自主开发，相关代码已经放在`EviMoni_Soft`目录下
* 硬件部分采用CubeMX+PlatformIO的方式开发，在CubeIDE上也同样通过了编译测试
* 目前软硬件部分代码均未检验正确性
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
