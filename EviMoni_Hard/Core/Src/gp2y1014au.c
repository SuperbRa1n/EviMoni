/*
 * GP2Y1014AU.c
 *
 *  Created on: 2023/2/9
 *      Author: zzy
 */

#include "gp2y1014au.h"
#include "adc.h"

float GetPMValue()
{
    float PMValue = 0.0;
    LED_LOW;
    DelayUs(280);
    PMValue = HAL_ADC_GetValue(&hadc2);
    DelayUs(40);
    LED_HIGH;
    DelayUs(9680);
    if (PMValue > 36.455)
    {
        return ((float)((PMValue / 1024) - 0.0356) * 120000 * 0.035); // 将读到的数据进行适当的转化
    }
    else
    {
        return -1; // 没读到数据，返回-1
    }
}