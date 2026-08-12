#include "stm32f4xx_hal.h"
#include "main.h"
#include "usart.h"
#include "Emm_V5.h"
#include "Motor.h"
#include "Motor_action.h"
extern uint8_t K230_Rx[8];
uint8_t K230_Rxflag=0;
uint8_t Command_Data[8];
__IO unsigned char target = 0;

//extern __IO uint8_t bean_color[2];//从左到右的颜色
//extern __IO uint8_t position[4];//从左到右的数字
/*黄0x07--1  绿--0x06--2  白0x05--3*/
uint8_t Data[8]={2,2,0,0,0,0,0,0};
void K230_DMA_Init(void)
{
  HAL_UART_Receive_DMA(&huart3, K230_Rx,8);//执行该函数跳转下面"HAL_UART_RxCpltCallback"回调函数

}

void K230_Command(uint8_t command)/*2开启看豆，3开启看数，1看侧面数字，6关闭摄像头*/
{
	Command_Data[0] = command;
	Command_Data[1] = 1;
	for (uint8_t i = 2; i < 8; i++)
		Command_Data[i] = 0;

	if(command==6)
		Command_Data[1] = 0;

	HAL_UART_Transmit(&huart3, Command_Data, 8, 100);
}

void Data_Handle1(uint8_t key)
{

    unsigned char pos = 0;
    unsigned char i;

    // 1、绑定对应目标值
    if(key == 0x06)      target = 2;   //06是绿豆 07是黄豆 08是白豆
    else if(key == 0x07) target = 1;  // 0x07 对应找数字 1
    else if(key == 0x08) target = 3;
    else                 return;

    // 2、遍历数组，查询目标值所在位置(1~5)
    for(i = 0; i < 5; i++)
    {
        if(number_position[i] == target)
        {
            pos = i + 1;
            break;
        }
    }

    // 3、五位位置 switch 判断，直接写你的动作代码
    switch(pos)
    {
        case 1:
					Action_1();
            // 第1位 执行逻辑
            break;
        case 2:
					Action_2();

            // 第2位 执行逻辑
            break;
        case 3:

					Action_3();

            // 第3位 执行逻辑
            break;
        case 4:
					Action_4();
            // 第4位 执行逻辑
            break;
        case 5:
					Action_5();
            // 第5位 执行逻辑
            break;
        default:
            break;
    }

}
void Data_Handle1_1(uint8_t key)
{

    unsigned char pos = 0;
    unsigned char i;

    // 1、绑定对应目标值
    if(key == 0x06)      target = 2;   //06是绿豆 07是黄豆 08是白豆
    else if(key == 0x07) target = 1;  // 0x07 对应找数字 1
    else if(key == 0x08) target = 3;
    else                 return;

    // 2、遍历数组，查询目标值所在位置(1~5)
    for(i = 0; i < 5; i++)
    {
        if(number_position[i] == target)
        {
            pos = i + 1;
            break;
        }
    }

    // 3、五位位置 switch 判断，直接写你的动作代码
    switch(pos)
    {
        case 1:
					Action_6();
            // 第1位 执行逻辑
            break;
        case 2:
					Action_7();
            // 第2位 执行逻辑
            break;
        case 3:
					Action_8();
            // 第3位 执行逻辑
            break;
        case 4:
					Action_9();
            // 第4位 执行逻辑
            break;
        case 5:
					Action_10();
            // 第5位 执行逻辑
            break;
        default:
            break;
    }

}
#define BEAN_YELLOW  0x07
#define BEAN_GREEN   0x06
#define BEAN_WHITE   0x08

void Data_Handle(uint8_t key, uint8_t mode, uint8_t *pos_array)
{
    unsigned char pos = 0;
    unsigned char i;
    unsigned char target = 0;

    // 1、绑定对应目标值
    if(key == 0x06)      target = 2;   //绿豆
    else if(key == 0x07) target = 1;   //黄豆
    else if(key == 0x08) target = 3;   //白豆
    else                 return;       //未知颜色直接退出

    // 2、遍历5位数组，查找目标位置
    for(i = 0; i < 5; i++)
    {
        if(pos_array[i] == target)
        {
            pos = i + 1; //pos范围：1~5
            break;
        }
    }

    // pos=0：数组找不到目标，直接退出
    if(pos == 0)
        return;

    // 3、根据位置+模式选择动作
    if(mode == 0)
    {
        switch(pos)
        {
            case 1: Action_1(); break;
            case 2: Action_2(); break;
            case 3: Action_3(); break;
            case 4: Action_4(); break;
            case 5: Action_5(); break;
        }
    }
    else if(mode == 1)
    {
        switch(pos)
        {
            case 1: Action_6(); break;
            case 2: Action_7(); break;
            case 3: Action_8(); break;
            case 4: Action_9(); break;
            case 5: Action_10(); break;
        }
    }
}

void Bean_Blink(uint8_t count)
{
    for (uint8_t i = 0; i < count; i++)
    {
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, GPIO_PIN_SET);
        HAL_Delay(300);
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, GPIO_PIN_RESET);
        HAL_Delay(300);
    }
    HAL_Delay(800);
}

void Bean_Show(void)
{

while(bean_flag==0);
    for (uint8_t i = 0; i < 3; i++)
    {
        if (bean_color[i] == BEAN_YELLOW)
            Bean_Blink(1);
        else if (bean_color[i] == BEAN_GREEN)
            Bean_Blink(2);
        else if (bean_color[i] == BEAN_WHITE)
            Bean_Blink(3);
    }

    bean_flag = 0;
    HAL_UART_Receive_DMA(&huart3, K230_Rx, 8);
}
