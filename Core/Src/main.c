/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "can.h"
#include "dma.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "servo.h"
#include "motor.h"
#include "string.h"
#include "k230.h"
#include "LobotServoController.h"
#include "Motor_action.h"
#include "Can_Motor.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
#define LED_PORT    GPIOB
#define LED_PIN     GPIO_PIN_2
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
 uint8_t Position_Rx[8];
uint8_t Motor_Rx[4];
uint8_t K230_Rx[8];
__IO uint8_t bean_flag=0;
__IO uint8_t position_flag = 0;
__IO uint8_t bean_locked=0;       // �?? 新�?�：bean_color �??
__IO uint8_t position_locked = 0;   // �?? 新�?�：number_position �??
uint8_t count=0;
/*�ⲿ�ж�*/
volatile uint32_t last_exti_tick = 0;    // �ϴ��жϴ���ʱ���??
volatile uint32_t count1 = 0;            // �ܴ�������������
volatile uint8_t exti_flag = 0;          // �жϱ�־λ��0=δ������1=�Ѵ�����
volatile uint8_t pin7_triggered = 0;     // PIN7 ����������־
volatile uint8_t pin8_triggered = 0;     // PIN8 ����������־
// ����ȫ�ֱ�������������
volatile uint8_t bean_color[3];       ////�����ҵ���ɫ ��0x01������ɫ������0-2��Ӧλ��1-3
volatile uint8_t number_position[5];  ////�����ҵ����� ��0x03����λ�ñ�ţ�??1-5
uint8_t flag_1;
 uint8_t flag_2;
// �������ṹ�壺��ɫ+��Ӧλ��

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */



/**
   * @brief  �?? LED �?烁验�? number_position[5] �?否�?�到真实数据
   *
   * 规则�??
   *   数组�?? i 位（箱子编号 1~5）的值是多少，LED 就闪多少下�??
   *   值为 0 表示该位�?没�?�到数据 �?? �?�? 10 下报警�??
   *   每显示完�??�?位置�? 1.5 秒，方便人眼区分�??
   *
   * 调用位置：K230 返回 0x03 指令后，number_position �?�?充完即可调用�??
   */
void Verify_NumberPosition(void)
 {
      for (uint8_t i = 0; i < 5; i++)
      {
          uint8_t val = number_position[i];

          /*
           * �??头提示：�?�? 1 下，表示"接下来是�?? i+1 �?箱�?"
           */
          HAL_GPIO_WritePin(LED_PORT, LED_PIN, GPIO_PIN_SET);
          HAL_Delay(100);
          HAL_GPIO_WritePin(LED_PORT, LED_PIN, GPIO_PIN_RESET);
          HAL_Delay(300);

          /*
           * 根据数�?�闪烁�?�应次�?
           */
          if (val == 0)
          {
              /* 没�?�到数据：快�? 10 下报�?? */
              for (uint8_t j = 0; j < 10; j++)
              {
                  HAL_GPIO_WritePin(LED_PORT, LED_PIN, GPIO_PIN_SET);
                  HAL_Delay(80);
                  HAL_GPIO_WritePin(LED_PORT, LED_PIN, GPIO_PIN_RESET);
                  HAL_Delay(80);
              }
          }
          else
          {
              /* 读到的是数字几就�?几�? */
              for (uint8_t j = 0; j < val; j++)
              {
                  HAL_GPIO_WritePin(LED_PORT, LED_PIN, GPIO_PIN_SET);
                  HAL_Delay(300);
                  HAL_GPIO_WritePin(LED_PORT, LED_PIN, GPIO_PIN_RESET);
                  HAL_Delay(300);
              }
          }

          /* 每个箱子之间停顿 3 �?? */
          HAL_Delay(3000);
      }
  }
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */




  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_USART1_UART_Init();
  MX_USART2_UART_Init();
  MX_USART6_UART_Init();
  MX_USART3_UART_Init();
  MX_UART4_Init();
  MX_UART5_Init();
  MX_CAN1_Init();
	CAN_Motor_Init();
  /* USER CODE BEGIN 2 */
 

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
      HAL_StatusTypeDef r = CAN_Send_MotorCurrent(1, 1000);

      if (r == HAL_OK)
          HAL_GPIO_TogglePin(LED_PORT, LED_PIN);
      else if (r == HAL_BUSY)
          HAL_GPIO_TogglePin(LED_PORT, LED_PIN);  /* BUSY也闪一�? */
      HAL_Delay(500);
      HAL_Delay(5);
  }
  /* USER CODE END WHILE */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 25;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{


 
if (huart->Instance == USART3)
      {
          if (K230_Rx[0] == 0x02)
          {
              /* �?有未锁�? �?? 数据非全�?? 才保�?? */
              if (!bean_locked && (K230_Rx[2] != 0 && K230_Rx[3] != 0 && K230_Rx[4] != 0))
              {
                  for (int i = 0; i < 3; i++)
                      bean_color[i] = K230_Rx[i + 2];
                  bean_locked = 1;   // �?? 锁住，后�?数据全忽�?
                  bean_flag = 1;
              }
          }
          else if (K230_Rx[0] == 0x01)
          {
						if(K230_Rx[2] != 0 && K230_Rx[3] != 0 && K230_Rx[4] != 0 && K230_Rx[5] != 0 && K230_Rx[6] != 0)
						{
									count++;
                  for (int i = 0; i < 5; i++)
                      number_position[i] = K230_Rx[i + 2];
							position_flag=1;
						}
              
          }
          HAL_UART_Receive_DMA(&huart3, K230_Rx, 8);
				}


if (huart->Instance == UART4) 
		{				
			  
			if(Motor_Rx[0]==0x03&&Motor_Rx[1]==0xFD)
			{
				if(x_ack==0) { x_ack=1; }
				else { x_flag=1; x_ack=0; }
			}
			if(Motor_Rx[0]==0x04&&Motor_Rx[1]==0xFD)
			{
				if(z_ack==0) { z_ack=1; }
				else { z_flag=1; z_ack=0; }
			}
			
						HAL_UART_Receive_DMA(&huart4, Motor_Rx, 4);
		}


}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  {
    
    if (HAL_GetTick() - last_exti_tick < 100) {
        return;
    }
    
    
    last_exti_tick = HAL_GetTick();
    
   
    count1++;
    
 
    exti_flag = !exti_flag;
    
   
    if (GPIO_Pin == GPIO_PIN_7) {
        
        pin7_triggered = 1;
        
        
    }
    else if (GPIO_Pin == GPIO_PIN_8) {
      
        pin8_triggered = 1;
        
      
    }
}
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
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
