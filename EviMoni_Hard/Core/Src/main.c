/* USER CODE BEGIN Header */
/**
<<<<<<< HEAD
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
=======
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2023 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "dma.h"
#include "i2c.h"
>>>>>>> 92956dcdca0283fbcfeaefc9c62cd579612e16a2
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
<<<<<<< HEAD
#include "delay.h"
=======
#include "ds18b20.h"
#include "gp2y1014au.h"
#include "sgp30.h"
#include "hc-sr04.h"
>>>>>>> 92956dcdca0283fbcfeaefc9c62cd579612e16a2
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define ADC_MAX_NUM 2
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
<<<<<<< HEAD
extern uint8_t ch;
float curTemp = 0.0;                    // ��ǰ�����¶�
float curPM = 0;                        // ��ǰ����PM2.5
uint16_t ADC_Values[ADC_MAX_NUM] = {0}; // �������к�����
float curYouhai = 0.0;                  // ��ǰ�����к�����
float curLight = 0.0;                   // ��ǰ������ǿ
uint16_t TVOC = 0, CO2 = 0;             // ��ǰCO2��TVOC
uint8_t ID[6] = {0};                    // SGP30��ID
unsigned int curLen;                    // ��ǰ����
=======
float curTemp = 0.0;                    // 当前环境温度
float curPM = 0;                        // 当前环境PM2.5
uint16_t ADC_Values[ADC_MAX_NUM] = {0}; // 光敏和有害气体
float curYouhai = 0.0;                  // 当前环境有害气体
float curLight = 0.0;                   // 当前环境光强
uint16_t TVOC = 0, CO2 = 0;             // 当前CO2和TVOC
uint8_t ID[6] = {0};                    // SGP30的ID
unsigned int curLen;                    // 当前距离
>>>>>>> 92956dcdca0283fbcfeaefc9c62cd579612e16a2
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
<<<<<<< HEAD
  * @brief  The application entry point.
  * @retval int
  */
=======
 * @brief  The application entry point.
 * @retval int
 */
>>>>>>> 92956dcdca0283fbcfeaefc9c62cd579612e16a2
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */
<<<<<<< HEAD

=======
>>>>>>> 92956dcdca0283fbcfeaefc9c62cd579612e16a2
  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
<<<<<<< HEAD
  MX_USART2_UART_Init();
  MX_USART1_UART_Init();
  MX_SPI2_Init();
  MX_TIM2_Init();
  /* USER CODE BEGIN 2 */
	HAL_UART_Receive_IT(&huart1,&ch,1);//ÿ�ν���һ���ֽھͲ���һ���ж�
=======
  MX_DMA_Init();
  MX_ADC1_Init();
  MX_ADC2_Init();
  MX_I2C1_Init();
  MX_SPI1_Init();
  MX_TIM1_Init();
  MX_TIM4_Init();
  MX_USART1_UART_Init();
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */
  while (sgp30_init() < 0)
  {
    printf(" sgp30 init fail\r\n");
    HAL_Delay(1000);
  }

  printf(" sgp30 init ok\r\n");

  if (sgp30_get_serial_id(ID) < 0)
  {
    printf(" sgp30 read serial id failed\r\n");
  }
  else
  {
    printf(" sgp30 read serial id is:");
    for (int i = 0; i < 6; i++)
      printf("%02X", ID[i]);
    printf("\r\n");
  }

  //
  // SGP30模块开机需要一定时间初始化，在初始化阶段读取的CO2浓度为400ppm，TVOC为0ppd且恒定不变。
  // 因此上电后每隔一段时间读取一次SGP30模块的值，如果CO2浓度为400ppm，TVOC为0ppd，直到SGP30模块初始化完成。
  // 初始化完成后刚开始读出数据会波动比较大，属于正常现象，一段时间后会逐渐趋于稳定。
  // 气体类传感器比较容易受环境影响，测量数据出现波动是正常的，可自行添加滤波函数。
  //

  printf(" sgp30 wait air for init");
  do
  {
    if (sgp30_read(&CO2, &TVOC) < 0)
    {
      printf("\r\n spg30 read failed");
    }
    else
    {
      printf(".");
    }
    HAL_Delay(500);
  } while (TVOC == 0 && CO2 == 400);

  printf("\r\n");
>>>>>>> 92956dcdca0283fbcfeaefc9c62cd579612e16a2
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
<<<<<<< HEAD
    while (1)
    {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
    }
=======
  while (1)
  {
    curTemp = Data_TransFormation();
    curPM = GetPMValue();
    HAL_ADC_Start_DMA(&hadc1, (uint32_t *)ADC_Values, ADC_MAX_NUM);
    curYouhai = (float)(ADC_Values[0] * 3.3 / 4096);
    curLight = (float)(ADC_Values[1] * 3.3 / 4096);
    sgp30_read(&CO2, &TVOC);
    Start(); // 启动超声波模块
    printf("超声波模块已启动\r\n");
    while (!Get())
      ;      // 当Echo返回0，等待,变为1继续
    start(); // 开始计时
    while (Get())
      ; // 当Echo为1计数并等待  ,变为0继续
    curLen = get_stop();
    HAL_Delay(1000);
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
>>>>>>> 92956dcdca0283fbcfeaefc9c62cd579612e16a2
  /* USER CODE END 3 */
}

/**
<<<<<<< HEAD
  * @brief System Clock Configuration
  * @retval None
  */
=======
 * @brief System Clock Configuration
 * @retval None
 */
>>>>>>> 92956dcdca0283fbcfeaefc9c62cd579612e16a2
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
<<<<<<< HEAD

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
=======
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
   * in the RCC_OscInitTypeDef structure.
   */
>>>>>>> 92956dcdca0283fbcfeaefc9c62cd579612e16a2
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
<<<<<<< HEAD

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
=======
  /** Initializes the CPU, AHB and APB buses clocks
   */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
>>>>>>> 92956dcdca0283fbcfeaefc9c62cd579612e16a2
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
<<<<<<< HEAD
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
    /* User can add his own implementation to report the HAL error return state */
    __disable_irq();
    while (1)
    {
    }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
    /* User can add his own implementation to report the file name and line number,
       ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
=======
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;
  PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV6;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc) // DMA采集完成中断服务函数
{
  HAL_ADC_Stop_DMA(&hadc1); // 关闭DMA的ADC采集
}
/* USER CODE END 4 */

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef USE_FULL_ASSERT
/**
 * @brief  Reports the name of the source file and the source line number
 *         where the assert_param error has occurred.
 * @param  file: pointer to the source file name
 * @param  line: assert_param error line source number
 * @retval None
 */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
>>>>>>> 92956dcdca0283fbcfeaefc9c62cd579612e16a2
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
