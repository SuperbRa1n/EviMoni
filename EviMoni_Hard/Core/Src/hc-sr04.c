#include "hc-sr04.h"

void Start(void) // 启动超声波模块
{
    HAL_GPIO_WritePin(TRIG_GPIO_Port, TRIG_Pin, 1); // 拉高
    DelayUs(15);                               // 15us延时
    HAL_GPIO_WritePin(TRIG_GPIO_Port, TRIG_Pin, 0); // 拉低
}

int Get(void)
{
    return HAL_GPIO_ReadPin(ECHO_GPIO_Port, ECHO_Pin); // 获得Echo的电平高低
}

void start(void) // 开始计时
{
    __HAL_TIM_SetCounter(&htim1, 0); // 清0开始计数
    HAL_TIM_Base_Start_IT(&htim1);   // 开启定时器更新中断
}

unsigned int get_stop(void) // 获取停止之后的数据
{
    unsigned int num, dta;
    HAL_TIM_Base_Stop_IT(&htim1);       // 关闭定时器更新中断
    num = __HAL_TIM_GetCounter(&htim1); // 获得计数值
    dta = (num * 346) / 20000;          // cm为单位
    return dta;
}

