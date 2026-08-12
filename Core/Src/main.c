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
__IO uint8_t bean_locked=0;       // ï¿?? æ–°å?žï¼šbean_color ï¿??
__IO uint8_t position_locked = 0;   // ï¿?? æ–°å?žï¼šnumber_position ï¿??
uint8_t count=0;
/*ï¿½â²¿ï¿½Ð¶ï¿½*/
volatile uint32_t last_exti_tick = 0;    // ï¿½Ï´ï¿½ï¿½Ð¶Ï´ï¿½ï¿½ï¿½Ê±ï¿½ï¿½ï¿??
volatile uint32_t count1 = 0;            // ï¿½Ü´ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½
volatile uint8_t exti_flag = 0;          // ï¿½Ð¶Ï±ï¿½Ö¾Î»ï¿½ï¿½0=Î´ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½1=ï¿½Ñ´ï¿½ï¿½ï¿½ï¿½ï¿½
volatile uint8_t pin7_triggered = 0;     // PIN7 ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ö¾
volatile uint8_t pin8_triggered = 0;     // PIN8 ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ö¾
// ï¿½ï¿½ï¿½ï¿½È«ï¿½Ö±ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½
volatile uint8_t bean_color[3];       ////ï¿½ï¿½ï¿½ï¿½ï¿½Òµï¿½ï¿½ï¿½É« ï¿½ï¿½0x01ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½É«ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½0-2ï¿½ï¿½Ó¦Î»ï¿½ï¿½1-3
volatile uint8_t number_position[5];  ////ï¿½ï¿½ï¿½ï¿½ï¿½Òµï¿½ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½0x03ï¿½ï¿½ï¿½ï¿½Î»ï¿½Ã±ï¿½Å£ï¿??1-5
uint8_t flag_1;
 uint8_t flag_2;
// ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½á¹¹ï¿½å£ºï¿½ï¿½É«+ï¿½ï¿½Ó¦Î»ï¿½ï¿½

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */



/**
   * @brief  ï¿?? LED é—?çƒéªŒè¯? number_position[5] æ˜?å¦è?»åˆ°çœŸå®žæ•°æ®
   *
   * è§„åˆ™ï¿??
   *   æ•°ç»„ï¿?? i ä½ï¼ˆç®±å­ç¼–å· 1~5ï¼‰çš„å€¼æ˜¯å¤šå°‘ï¼ŒLED å°±é—ªå¤šå°‘ä¸‹ï¿½??
   *   å€¼ä¸º 0 è¡¨ç¤ºè¯¥ä½ç½?æ²¡è?»åˆ°æ•°æ® ï¿?? å¿?é—? 10 ä¸‹æŠ¥è­¦ï¿½??
   *   æ¯æ˜¾ç¤ºå®Œï¿??ä¸?ä½ç½®å? 1.5 ç§’ï¼Œæ–¹ä¾¿äººçœ¼åŒºåˆ†ï¿??
   *
   * è°ƒç”¨ä½ç½®ï¼šK230 è¿”å›ž 0x03 æŒ‡ä»¤åŽï¼Œnumber_position è¢?å¡?å……å®Œå³å¯è°ƒç”¨ï¿??
   */
void Verify_NumberPosition(void)
 {
      for (uint8_t i = 0; i < 5; i++)
      {
          uint8_t val = number_position[i];

          /*
           * ï¿??å¤´æç¤ºï¼šå¿?é—? 1 ä¸‹ï¼Œè¡¨ç¤º"æŽ¥ä¸‹æ¥æ˜¯ï¿?? i+1 ä¸?ç®±ï¿½?"
           */
          HAL_GPIO_WritePin(LED_PORT, LED_PIN, GPIO_PIN_SET);
          HAL_Delay(100);
          HAL_GPIO_WritePin(LED_PORT, LED_PIN, GPIO_PIN_RESET);
          HAL_Delay(300);

          /*
           * æ ¹æ®æ•°ï¿½?ï¿½é—ªçƒå?¹åº”æ¬¡ï¿½?
           */
          if (val == 0)
          {
              /* æ²¡è?»åˆ°æ•°æ®ï¼šå¿«é—? 10 ä¸‹æŠ¥ï¿?? */
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
              /* è¯»åˆ°çš„æ˜¯æ•°å­—å‡ å°±é—?å‡ ï¿½? */
              for (uint8_t j = 0; j < val; j++)
              {
                  HAL_GPIO_WritePin(LED_PORT, LED_PIN, GPIO_PIN_SET);
                  HAL_Delay(300);
                  HAL_GPIO_WritePin(LED_PORT, LED_PIN, GPIO_PIN_RESET);
                  HAL_Delay(300);
              }
          }

          /* æ¯ä¸ªç®±å­ä¹‹é—´åœé¡¿ 3 ï¿?? */
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
          HAL_GPIO_TogglePin(LED_PORT, LED_PIN);  /* BUSYä¹Ÿé—ªä¸€ä¸? */
      HAL_Delay(500);
      HAL_Delay(1000);
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
              /* å?æœ‰æœªé”ï¿½? ï¿?? æ•°æ®éžå…¨ï¿?? æ‰ä¿ï¿?? */
              if (!bean_locked && (K230_Rx[2] != 0 && K230_Rx[3] != 0 && K230_Rx[4] != 0))
              {
                  for (int i = 0; i < 3; i++)
                      bean_color[i] = K230_Rx[i + 2];
                  bean_locked = 1;   // ï¿?? é”ä½ï¼ŒåŽç»?æ•°æ®å…¨å¿½ç•?
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
