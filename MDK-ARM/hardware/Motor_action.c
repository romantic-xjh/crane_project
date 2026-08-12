#include "stm32f4xx_hal.h"
#include "servo.h"
#include "motor.h"
#include "string.h"
#include "k230.h"
#include "LobotServoController.h"
#include "dma.h"
#include "usart.h"
#include "gpio.h"
#include "main.h"
#include "Motor_action.h"

uint8_t Count_data=1;
//K230_Command(3);	
//Servo_Close(1);
//Motor_XYZ(0, 300, 180,480,
//             1, 180, 180, 860,
//               0, 700, 180, 110);
//							 K230_Command(2);
//Uart_Forward(300,220,450);
// K230_Command(6);												/*2开启看豆，3开启看数，1看侧面数字，6关闭摄像头*/
//Uart_Down(700,180,70);

// HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, GPIO_PIN_SET);
//Uart_Down(300,180,33);
//Uart_Right(20,180,20);
//HAL_GPIO_WritePin(GPIOB,GPIO_PIN_9,GPIO_PIN_RESET);
//Uart_Up(700,180,103);
//Uart_Backward(300,220,950);
//Servo_Rotate90(1);
// K230_Command(1);
//Motor_XYZ(1, 500, 180,860,
//             0, 380, 180, 2200,
//               0, 700, 180, 30);

void Action_1(void)
{	
	flag_1=1;
	Motor_XY(1, 300, 180,500,
             1, 180, 180, 250);//微调位置
	Servo_Open(1);
	HAL_Delay(2000);
	if(target ==2)
{

	Uart_Left(450, 0,40,500);
	Uart_Right(450, 0,40,500);
		Uart_Left(450, 0,40,500);
	Uart_Right(450, 0,40,500);

	}
HAL_Delay(500);

	Servo_Close(1);
	Uart_Left(500, 180,500,4500);//回到微调之前的指定1位置
	
}


void Action_2(void)
{
	flag_1=1;
	Uart_Forward(180, 180,25,2500);
	Servo_Open(1);
	HAL_Delay(2000);
	if(target ==2)
{
	Uart_Left(450, 0,40,500);
	Uart_Right(450, 0,40,500);
		Uart_Left(450, 0,40,500);
	Uart_Right(450, 0,40,500);
	}
HAL_Delay(500);

	Servo_Close(1);
Uart_Forward( 500, 180, 225,2500);//回到微调之前的指定1位置
	
}


void Action_3(void)
{
	flag_1=1;
	Motor_XY(0, 500, 180,410,
            1, 180, 180,25);//微调位置
	Servo_Open(1);
	HAL_Delay(2000);
	if(target ==2)
{
	Uart_Left(450, 0,40,500);
	Uart_Right(450, 0,40,500);
		Uart_Left(450, 0,40,500);
	Uart_Right(450, 0,40,500);
	}
HAL_Delay(500);
	
	Servo_Close(1);
	Motor_XY(1, 500, 180,410,
             1, 500, 180, 225);//回到微调之前的指定1位置
	
}


void Action_4(void)
{
	flag_1=2;
	Motor_XY(0, 500, 180,820,
            1, 180, 180,25);//微调位置
	Servo_Open(1);
	HAL_Delay(2000);
	if(target ==2)
{
	Uart_Left(450, 0,40,700);
	Uart_Right(450, 0,40,700);
	
	}


	Servo_Close(1);
	Uart_Forward( 500, 180, 225,2500);//回到微调之前的指定1位置
	
}


void Action_5(void)
{
	flag_1=2;
	Motor_XY(0, 500, 180,1320,
             1, 180, 180, 250);//微调位置
	Servo_Open(1);
	HAL_Delay(2000);
	if(target ==2)
{
Uart_Right(450, 0,40,500);
	Uart_Left(450, 0,40,500);
	Uart_Right(450, 0,40,500);
	Uart_Left(450, 0,40,500);

	}
HAL_Delay(500);

	Servo_Close(1);
	Uart_Right(500, 180,495,4000);

	
	
}

void Action_10(void)
{
	flag_2=2;
	Motor_XZ(0, 500, 180,474,
               0, 700, 180, 10);//微调位置
	Servo_Open(1);
	HAL_Delay(2000);
	if(target ==2)
{
Uart_Right(450, 0,20,500);
	Uart_Left(450, 0,40,500);
	Uart_Right(450, 0,40,500);
	Uart_Left(450, 0,20,500);
	}
HAL_Delay(500);

	Servo_Close(1);
	Motor_XZ(1, 500, 180,474,
               1, 700, 180, 10);//回到微调之前的指定2位置
	Count_data++;
	if(Count_data==3)
	{
		flag_2=0;
		Count_data=1;
	}
	
}
void Action_9(void)
{
	flag_2=2;
Motor_Y(0, 180, 180, 230);
		HAL_Delay(2000);
	Servo_Open(1);
	HAL_Delay(2000);
	if(target ==2)
{
Uart_Right(450, 0,20,500);
	Uart_Left(450, 0,40,500);
	Uart_Right(450, 0,40,500);
	Uart_Left(450, 0,20,500);
	}
HAL_Delay(500);

	Servo_Close(1);
Uart_Forward( 500, 180, 230,2500);
	Count_data++;
	if(Count_data==3)
	{
		flag_2=0;
		Count_data=1;
	}
	
}
void Action_8(void)
{
	flag_2=2;
	Motor_XY(1, 500, 180,420,
             0, 180, 180, 230);
	Servo_Open(1);
	HAL_Delay(2000);
	if(target ==2)
{
Uart_Right(450, 0,20,500);
	Uart_Left(450, 0,40,500);
	Uart_Right(450, 0,40,500);
	Uart_Left(450, 0,20,500);
	}
HAL_Delay(500);

	Servo_Close(1);
	Motor_XY(0, 500, 180,420,
          1, 500, 180, 230);
	Count_data++;
	if(Count_data==3)
	{
		flag_2=0;
		Count_data=1;
	}
	
}
void Action_7(void)
{
	flag_2=1;
	Motor_XY(1, 500, 180,820,
             0, 180, 180, 230);
	Servo_Open(1);
	HAL_Delay(2000);
	if(target ==2)
{
Uart_Right(450, 0,20,500);
	Uart_Left(450, 0,40,500);
	Uart_Right(450, 0,40,500);
	Uart_Left(450, 0,20,500);
	}
HAL_Delay(500);

	Servo_Close(1);
	Uart_Forward( 500, 180, 230,2500);
	
	Count_data++;
	if(Count_data==3)
	{
		flag_2=0;
		Count_data=1;
	}
}
void Action_6(void)
{
	flag_2=1;
Uart_Right( 500, 180,1328,7500);
	Servo_Open(1);
	HAL_Delay(2000);
	if(target ==2)
{

Uart_Right(450, 0,20,500);
	Uart_Left(450, 0,40,500);
	Uart_Right(450, 0,40,500);
	Uart_Left(450, 0,20,500);

	}
HAL_Delay(500);
	

	
	Servo_Close(1);
Uart_Left( 500, 180,513,4500);//回到微调之前的指定2位置
            
	
	Count_data++;
	if(Count_data==3)
	{
		flag_2=0;
		Count_data=1;
	}
}


//这是完整执行第一步的代码
void frame_1(void)
{

	Serio_face(1);
	bean_locked = 0;//解锁，目的是为了成功读取到数据
	K230_Command(2);

	Motor_XYZ(0, 300, 180,480,
							 1, 240, 180, 1780,
								 0, 700, 240, 130);
	//这里的y轴速度不能改太大，会影响视觉那边的读取
	//如果以电机为标准起始线就是1750，如果是以大炮前面的桶为起始线就是1840
	
	K230_Command(6);
	
	switch(bean_color[0])
	{
		case 0x06:
			bean_action_G1();
			break;
		case 0x07:
			bean_action_Y1();	
			break;
		case 0x08:
			bean_action_W1();	
			break;
		default:
            break;
	
	}
		
	

	Uart_Backward(500,220,1000,4000);
	Servo_Rotate90(1);
	

			K230_Command(3);
	Motor_XYZ6(1, 400, 200,990,
             0, 240, 185, 1600,
								0, 700, 180, 10,6000);
		HAL_Delay(1500);
	K230_Command(6);
	Uart_Backward(220, 180, 670,4000);
	HAL_Delay(1200);
	K230_Command(1);

while(position_flag==0);
	position_flag=1;

Data_Handle1(bean_color[0]);

	}

void frame_2(void)
{
	//在指定位置的左侧
	if(flag_1==1)
	{Uart_Left(300,220,40,1000);
		Serio_face(1);
		Motor_XYZ(1, 300, 180,40,
             1, 600, 180, 3000,
              0, 700, 180, 75);
							
		switch(bean_color[2])
	{
		case 0x06:
			bean_action_G3();
			break;
		case 0x07:
			bean_action_Y3();	
			break;
		case 0x08:
			bean_action_W3();	
			break;
		default:
      break;
	}
	Motor_XYZ(0, 300, 180,40,
             0, 300, 180, 1000,
              0, 700, 180, 10);
	Servo_Rotate90(1);
	Motor_XYZ(0, 300, 180,780,
							0, 300, 180, 2000,
								1, 700, 180, 85);

		flag_1=0;
		Data_Handle1_1(bean_color[2]);
		
	}
	//在指定位置的右侧
	if(flag_1==2)
	{
		
Uart_Forward( 500, 180, 1000,4000);
		Serio_face(1);
		Motor_XYZ(1, 300, 180,800,
             1, 300, 180, 2000,
              0, 700, 180, 75);
					
		switch(bean_color[2])
	{
		case 0x06:
			bean_action_G3();
			break;
		case 0x07:
			bean_action_Y3();	
			break;
		case 0x08:
			bean_action_W3();	
			break;
		default:
      break;
	}
	Motor_YZ(
             0, 300, 180, 1000,
              0, 700, 180, 10);
	Servo_Rotate90(1);
	Motor_XYZ(0, 300, 180,800,
							0, 300, 180, 2000,
								1, 700, 180, 85);

		flag_1=0;
		Data_Handle1_1(bean_color[2]);
		
	}
	

}






void frame_3(void)
{
	//当位置停在左侧
	if(flag_2==1)
	{
		

		Uart_Left(300,220,40,1000);
		Serio_face(1);
		Motor_XYZ(1, 300, 180,40,
							1, 500, 180, 2540,
							0, 700, 180, 90);
		
		Motor_XY(0, 400, 180,505,
										1, 500, 180, 235);
		
		switch(bean_color[1])
	{
		case 0x06:
			bean_action_G2_1();
			break;
		case 0x07:
			bean_action_Y2_1();	
			break;
		case 0x08:
			bean_action_W2_1();	
			break;
		default:
			break;
	}
		Motor_XY(1, 500, 180,500,
										0, 100, 180, 485);
		Servo_Rotate90(1);
		Motor_XYZ(0, 400, 180,820,
										 0, 300, 180, 2310,
											1, 700, 180, 90);
		flag_2=0;									
		Data_Handle1_1(bean_color[1]);
	}
	


//当指定位置停在右侧
		if(flag_2==2)
		{
			Serio_face(1);
			Motor_YZ(
										 1, 500, 220, 2540,
											0, 700, 180, 90);
			
			Motor_XY(1, 400, 180,285,
										 1, 500, 180, 235);

			switch(bean_color[1])
	{
		case 0x06:
			bean_action_G2_2();
			break;
		case 0x07:
			bean_action_Y2_2();	
			break;
		case 0x08:
			bean_action_W2_2();	
			break;
		default:
      break;
	}
			Motor_XY(1, 500, 180,415,
										0, 100, 180, 480);
			Servo_Rotate90(1);
			Motor_XYZ(0, 400, 180,710,
										 0, 300, 180, 2310,
											1, 700, 180, 90);
		flag_2=0;
		Data_Handle1_1(bean_color[1]);
	}
}

//给一层吸豆
void bean_action_G1(void)
{
	Uart_Down(700,180,114);		
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, GPIO_PIN_SET);
Uart_Right(50, 180,60,2000);
	HAL_Delay(400);
	Uart_Left(50, 180,120,3000);
	HAL_Delay(400);
	Motor_XZ(1, 100, 180,60,
								 0,700,180,14);
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_9,GPIO_PIN_RESET);
	Uart_Up(700,180,100);
}


void bean_action_Y1(void)
{
	Uart_Down(700,180,116);		
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, GPIO_PIN_SET);
	Uart_Left(50, 180,70,2000);	
	Uart_Right(80, 180,140,3000);HAL_Delay(200);
	Uart_Left(50, 180,70,2000);	
HAL_GPIO_WritePin(GPIOB,GPIO_PIN_9,GPIO_PIN_RESET);
	Uart_Up(700,180,116);
}

void bean_action_W1(void)
{
//	Uart_Down(700,180,120);		
//	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, GPIO_PIN_SET);

//	HAL_Delay(2000);
//	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_9,GPIO_PIN_RESET);
//	
//	Motor_XYZ(0, 300, 180,90,
//							 0, 0, 0, 0,
//								 0, 0,0, 0);
//	HAL_Delay(500);
//	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, GPIO_PIN_SET);

//	HAL_Delay(2000);
//	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_9,GPIO_PIN_RESET);
//	
//	Motor_XYZ(1, 300, 180,125,
//							 0, 0, 0, 0,
//								 0, 0,0, 0);
//	HAL_Delay(500);
//	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, GPIO_PIN_SET);

//	HAL_Delay(2000);
//	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_9,GPIO_PIN_RESET);
//	Motor_XYZ(0, 300, 180,35,
//							 0, 0, 0, 0,
//								 0,700,180,120);
								 
Uart_Down(700,180,105);		
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, GPIO_PIN_SET);
Uart_Down(700,180,10);	
	
	Uart_Left(50, 180,70,2000);	HAL_Delay(400);
	Uart_Right(60, 180,140,3000);	HAL_Delay(400);
	Uart_Left(50, 180,70,2000);	



	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_9,GPIO_PIN_RESET);

Uart_Up(700,180,115);

}

//给三层吸豆
void bean_action_G2_1(void)//左侧
{
	Uart_Down(700,180,110);		
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, GPIO_PIN_SET);
Uart_Left(60, 180,80,2000);HAL_Delay(200);
	Uart_Right(60, 180,140,3000);HAL_Delay(300);
		Motor_XZ(0, 60, 180,60,
								 0,700,180,15);
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_9,GPIO_PIN_RESET);

		Uart_Up(700,180,95);


}


void bean_action_G2_2(void)//右侧
{
	Uart_Down(700,180,110);		
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, GPIO_PIN_SET);
Uart_Left(60, 180,80,2000);HAL_Delay(200);
	Uart_Right(60, 180,140,3000);HAL_Delay(200);
	Motor_XZ(0, 60, 180,60,
								 0,700,180,15);
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_9,GPIO_PIN_RESET);

	Uart_Up(700,180,95);


}

void bean_action_W2_1(void)//左侧
{
	Uart_Down(700,180,108);		
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, GPIO_PIN_SET);
	
	Uart_Right( 50, 180,80,2000);HAL_Delay(400);
	Uart_Down(700,180,5);	
	Uart_Left( 50, 180,140,3000);HAL_Delay(400);

	Uart_Right( 80, 180,60,2000);
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_9,GPIO_PIN_RESET);
	Uart_Up( 700, 180, 113);

}

void bean_action_W2_2(void)//右侧
{
	Uart_Down(700,180,108);		
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, GPIO_PIN_SET);

	Uart_Right(50, 180,80,2000);HAL_Delay(400);
		Uart_Down(700,180,5);	
Uart_Left( 50, 180,140,3000);HAL_Delay(400);
	Uart_Right( 80, 180,60,2000);
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_9,GPIO_PIN_RESET);
	Uart_Up( 700, 180, 113);
}

void bean_action_Y2_1(void)//左侧
{
	Uart_Down(700,180,110);		
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, GPIO_PIN_SET);

Uart_Right( 60, 180,70,2000);
Uart_Left(60, 180,140,2000);
Uart_Right( 60, 180,70,2000);

			HAL_GPIO_WritePin(GPIOB,GPIO_PIN_9,GPIO_PIN_RESET);
		Uart_Up(700,180,110);

}

void bean_action_Y2_2(void)//右侧
{
	Uart_Down(700,180,110);		
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, GPIO_PIN_SET);

Uart_Right( 60, 180,70,2000);
Uart_Left(60, 180,140,2000);
Uart_Right( 60, 180,70,2000);
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_9,GPIO_PIN_RESET);
	Uart_Up(700,180,110);

}

//给二层吸豆
void bean_action_G3(void)
{
	Uart_Down(700,240,154);		
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, GPIO_PIN_SET);
Uart_Left(60, 180,80,2000);HAL_Delay(200);
	Uart_Right(80, 180,130,3000);HAL_Delay(200);
	Motor_XZ(0, 100, 180,50,
								 0,700,180,10);
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_9,GPIO_PIN_RESET);
	

	Uart_Up(700,180,144);
}

void bean_action_W3(void)
{
	Uart_Down(700,240,145);		
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, GPIO_PIN_SET);

Uart_Right( 50, 180,60,2000);HAL_Delay(200);
	Motor_XZ(0, 40, 180,120,
								 1, 700, 180, 10);
Uart_Right( 50, 180,60,2000);

HAL_Delay(400);

	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_9,GPIO_PIN_RESET);
	Uart_Up(700,240,155);
}

void bean_action_Y3(void)
{
	Uart_Down(700,240,153);		
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, GPIO_PIN_SET);
Uart_Right(  80, 180,80,2000);

	Uart_Left(80, 180,110,2000);

Uart_Right( 80, 180,30,2000);

	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_9,GPIO_PIN_RESET);
	Uart_Up(700,240,153);
}

/**
 * @brief 通用抓取任务函数
 * @param c1 第0路豆子颜色 bean_color[0]
 * @param c2 第1路豆子颜色 bean_color[1]
 * @param c3 第2路豆子颜色 bean_color[2]
 * @param pos_array 传入5字节位置数组 number_position
 */
void task(uint8_t c1, uint8_t c2, uint8_t c3, uint8_t *pos_array)
{
target =2;
	Serio_face(1);
	bean_locked = 0;//解锁，目的是为了成功读取到数据
	K230_Command(2);

	Motor_XYZ(0, 300, 180,480,
							 1, 240, 180, 1780,
								 0, 700, 240, 130);

    switch(c1)
    {
        case 0x06:
            bean_action_G1();
            break;
        case 0x07:
            bean_action_Y1();
            break;
        case 0x08:
            bean_action_W1();
            break;
        default:
            break;
    }

    Uart_Backward(300,220,1000,4000);
    Servo_Rotate90(1);

    Motor_XYZ(1, 350, 200,990,
              0, 220, 180, 1600,
              0, 700, 180, 10);

    Uart_Backward(180, 180, 660,4000);
Data_Handles(c1, 0, pos_array);
    //在指定位置的左侧
    if(flag_1==1)
    {
        Uart_Left(300,220,40,1000);
        Serio_face(1);
        Motor_XYZ(1, 300, 180,40,
                  1, 600, 180, 2995,
                  0, 700, 180, 75);

        switch(c3)
        {
            case 0x06:
                bean_action_G3();
                break;
            case 0x07:
                bean_action_Y3();
                break;
            case 0x08:
                bean_action_W3();
                break;
            default:
                break;
        }
        Motor_XYZ(0, 300, 180,40,
                  0, 300, 180, 1000,
                  0, 700, 180, 10);
        Servo_Rotate90(1);
        Motor_XYZ(0, 300, 180,780,
                  0, 300, 180, 2000,
                  1, 700, 180, 85);

        flag_1=0;
        //mode=1 等价旧 Data_Handle1_1
        Data_Handles(c3, 1, pos_array);
    }

    //在指定位置的右侧
    if(flag_1==2)
    {
        Uart_Forward( 500, 180, 995,4000);
        Serio_face(1);
        Motor_XYZ(1, 300, 180,800,
                  1, 300, 180, 2000,
                  0, 700, 180, 75);

        switch(c3)
        {
            case 0x06:
                bean_action_G3();
                break;
            case 0x07:
                bean_action_Y3();
                break;
            case 0x08:
                bean_action_W3();
                break;
            default:
                break;
        }
        Motor_YZ(0, 300, 180, 1000,
                  0, 700, 180, 10);
        Servo_Rotate90(1);
        Motor_XYZ(0, 300, 180,800,
                  0, 300, 180, 2000,
                  1, 700, 180, 85);

        flag_1=0;
        Data_Handles(c3, 1, pos_array);
    }

    //当位置停在左侧
    if(flag_2==1)
    {
        Uart_Left(300,220,40,1000);
        Serio_face(1);
        Motor_XYZ(1, 300, 180,40,
                  1, 500, 180, 2530,
                  0, 700, 180, 90);

        Motor_XY(0, 400, 180,505,
                 1, 500, 180, 235);

        switch(c2)
        {
            case 0x06:
                bean_action_G2_1();
                break;
            case 0x07:
                bean_action_Y2_1();
                break;
            case 0x08:
                bean_action_W2_1();
                break;
            default:
                break;
        }
        Motor_XY(1, 500, 180,500,
                 0, 100, 180, 485);
        Servo_Rotate90(1);
        Motor_XYZ(0, 300, 180,840,
                  0, 300, 180, 2300,
                  1, 700, 180, 90);
        flag_2=0;
        Data_Handles(c2, 1, pos_array);
    }

    //当指定位置停在右侧
    if(flag_2==2)
    {
        Serio_face(1);
        Motor_YZ(1, 500, 220, 2530,
                 0, 700, 180, 90);

        Motor_XY(1, 400, 180,285,
                 1, 500, 180, 235);

        switch(c2)
        {
            case 0x06:
                bean_action_G2_2();
                break;
            case 0x07:
                bean_action_Y2_2();
                break;
            case 0x08:
                bean_action_W2_2();
                break;
            default:
                break;
        }
        Motor_XY(1, 500, 180,415,
                 0, 100, 180, 480);
        Servo_Rotate90(1);
        Motor_XYZ(0, 300, 180,710,
                  0, 300, 180, 2305,
                  1, 700, 180, 90);
        flag_2=0;
        Data_Handles(c2, 1, pos_array);
    }
}
/**
 * @brief 根据豆子类型，在5位位置数组查找目标，根据模式执行对应动作
 * @param key         豆子识别码 0x06绿豆,0x07黄豆,0x08白豆
 * @param mode        模式 0：Action1~5    1：Action6~10
 * @param pos_array   外部传入5字节位置数组 uint8_t number_position[5]
 */
void Data_Handles(uint8_t key, uint8_t mode, uint8_t *pos_array)
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
