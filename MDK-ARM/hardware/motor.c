#include "main.h"
#include "math.h"
#include "Emm_V5.h"
#include "stdlib.h"
#include "stdio.h"
#include "motor.h"
#include "bool.h"
#include "usart.h"

int fputc(int ch, FILE *f) {
HAL_UART_Transmit(&huart2, (uint8_t *)&ch, 1, 0xffff);
return ch;
}

float move_distance;
__IO uint8_t x_flag;
__IO uint8_t z_flag;
__IO uint8_t x_ack;  /* X轴(电机3)命令收到应答 */
__IO uint8_t z_ack;  /* Z轴(电机4)命令收到应答 */
int32_t sum_position;

__IO uint32_t uw_Tick_Motor = 0;
float Gear(int zhijing)
{
    return 3.14159 * zhijing;
}

void DMA_motor_Init()
{	__HAL_UART_CLEAR_IDLEFLAG(&huart2);
	__HAL_UART_ENABLE_IT(&huart2, UART_IT_IDLE);
  HAL_UART_Receive_DMA(&huart2, (uint8_t *)rxCmd, CMD_LEN);
  HAL_UART_Receive_DMA(&huart4,  Motor_Rx, 4);
}

int distancetoclk(int distance)
{
    int clk = 0;
 clk = (distance / Gear(23.11)) * 3200;

    return clk;
}


/*-----------------------若需要串口控制多个电机------------------------*/
/**
 * @brief    位置模式
 * @param    dir ：方向        ，0为CW，其余值为CCW
 * @param    vel ：速度(RPM)   ，范围0 - 5000RPM
 * @param    acc ：加速度      ，范围0 - 255，注意：0是直接启动
 * @param    distance :行走的距离(mm)      ，范围0- (2^32 - 1)个
 */

void Uart_Forward(uint16_t vel, uint8_t acc, float distance, uint32_t wait_timeout_ms)
{
    uint32_t clk = distancetoclk(distance);
    Emm_V5_Pos_Control(1, 1, vel, acc, clk, 0, 1);
    Emm_V5_Pos_Control(2, 1, vel, acc, clk, 0, true);

    Motorflag = 0;
    Emm_V5_Synchronous_motion(0);
    HAL_Delay(50);

    uint32_t wait_start = HAL_GetTick();
    while (1)
    {
        // 正常收到到位标志，退出
        if (Motorflag == 1)
        {
            break;
        }
        // 等待超时强制跳出，上报超时
        if (HAL_GetTick() - wait_start >= wait_timeout_ms)
        {

            break;
        }

        // 你原来预留的限位停止逻辑，可以恢复启用
//						if(pin8_triggered)
//						{
//							Motor_SetStop(1);
//							pin8_triggered=0;
//							break;
//						}
        HAL_Delay(5);
    }

    Motorflag = 0;
}

void Uart_Backward(uint16_t vel, uint8_t acc, float distance, uint32_t wait_timeout_ms)
{
    uint32_t clk = distancetoclk(distance);
    Emm_V5_Pos_Control(1, 0, vel, acc, clk, 0, true);
    Emm_V5_Pos_Control(2, 0, vel, acc, clk, 0, true);

    Motorflag = 0;
    Emm_V5_Synchronous_motion(0);

    uint32_t wait_start = HAL_GetTick();
    while (1)
    {
        if (Motorflag == 1)
        {
            break;
        }
        // 超时判断
        if (HAL_GetTick() - wait_start >= wait_timeout_ms)
        {
            uint8_t number_position[5] = {0,0,0,0x02,0};
            HAL_UART_Transmit(&huart1, number_position, 5, 10);
            break;
        }

        // 保留你原来限位触发逻辑，需要直接取消注释
        /*
        if(pin7_triggered==1||pin8_triggered==1)
        {
            Motor_SetStop(1);
            pin7_triggered=0;
            pin8_triggered=0;
            break;
        }
        */
        HAL_Delay(5);
    }

    Motorflag = 0;
}

/*-----------------------应答式发送, 超时重发------------------------*/
/**
 * @brief  发送Emm_V5位置指令并等待命令收到应答, 超时重发
 *         区分: ack=驱动器收到命令的应答, flag=电机运行完成的应答
 * @param  addr      电机地址(3=X, 4=Z)
 * @param  dir       方向
 * @param  vel       速度
 * @param  acc       加速度
 * @param  clk       脉冲数
 * @param  max_retry 最大重试次数
 * @retval 0=成功, 1=超时失败
 */
static uint8_t SendAndWait(uint8_t addr, uint8_t dir, uint16_t vel, uint8_t acc, uint32_t clk, uint8_t max_retry)
{
    __IO uint8_t *pack = (addr == 3) ? &x_ack : &z_ack;

    for (uint8_t retry = 0; retry < max_retry; retry++)
    {
        *pack = 0;
        Emm_V5_Pos(addr, dir, vel, acc, clk, 0, 0);
        HAL_Delay(200);
        /* 等待命令收到应答, 超时200ms */
        uint32_t tick_start = HAL_GetTick();
        while (*pack == 0)
        {
            if (HAL_GetTick() - tick_start > 200)	
							break;
						
                
        }

        if (*pack != 0)
            return 0;  /* 驱动器确认收到命令 */
				
    }
    return 1;  /* 重试耗尽, 失败 */
}

void Uart_Left(uint16_t vel, uint8_t acc, float distance, uint32_t wait_timeout_ms)
{
    uint32_t clk = 0;
    x_flag = 0;
    x_ack = 0;

    if(distance > 0.001f)
    {
        clk = (distance / Gear(72)) * 320 * 37;
        SendAndWait(3, 0, vel, acc, clk, 3);
        HAL_Delay(30);
    }
    else
    {
        x_flag = 1;
    }

    uint32_t wait_start = HAL_GetTick();
    while (1)
    {
        if (x_flag == 1)
        {
            break;
        }
        if (HAL_GetTick() - wait_start >= wait_timeout_ms)
        {
            uint8_t number_position[5] = {0,0,0,0x02,0};
            HAL_UART_Transmit(&huart1, number_position, 5, 10);
            break;
        }
        HAL_Delay(5);
    }
    x_flag = 0;
}

void Uart_Right(uint16_t vel, uint8_t acc, float distance, uint32_t wait_timeout_ms)
{
    uint32_t clk = 0;
    x_flag = 0;
    x_ack = 0;

    if(distance > 0.001f)
    {
        clk = (distance / Gear(72)) * 320 * 37;
        SendAndWait(3, 1, vel, acc, clk, 3);
        HAL_Delay(30);
    }
    else
    {
        x_flag = 1;
    }

    uint32_t wait_start = HAL_GetTick();
    while (1)
    {
        if (x_flag == 1)
        {
            break;
        }
        if (HAL_GetTick() - wait_start >= wait_timeout_ms)
        {
            uint8_t number_position[5] = {0,0,0,0x02,0};
            HAL_UART_Transmit(&huart1, number_position, 5, 10);
            break;
        }
        HAL_Delay(5);
    }
    x_flag = 0;
}


void Motor_SetStop(uint8_t adr)
{ if(adr==1)
	{Emm_V5_Stop_Now(1,true );
	Emm_V5_Stop_Now(2,true );
		Emm_V5_Synchronous_motion(0);}
		else
	Emm_V5_Stop_Now(adr,false );

}


void Uart_Up(uint16_t vel, uint8_t acc, float distance)
{
    uint32_t clk = (distance / Gear(24)) * 3200*14;
    z_flag = 0;
    z_ack = 0;
    SendAndWait(4, 0, vel, acc, clk, 3);
    HAL_Delay(30);

    while (z_flag == 0);
    z_flag = 0;
}

void Uart_Down(uint16_t vel, uint8_t acc, float distance)
{
    uint32_t clk = (distance / Gear(24)) * 3200*14;
    z_flag = 0;
    z_ack = 0;
    SendAndWait(4, 1, vel, acc, clk, 3);
    HAL_Delay(30);

    while (z_flag == 0);
    z_flag = 0;
}

/*-----------------------XYZ非阻塞移动函数------------------------*/
/**
 * @brief  X轴移动(左右旋转, 电机3), 非阻塞
 * @param  dir   方向: 0=左, 1=右
 * @param  vel   速度(RPM)
 * @param  acc   加速度
 * @param  distance 距离(mm)
 */
void Motor_X(uint8_t dir, uint16_t vel, uint8_t acc, float distance)
{
    uint32_t clk = (distance / Gear(72)) * 320 * 37;
    x_flag = 0;
    x_ack = 0;
    Emm_V5_Pos(3, dir, vel, acc, clk, 0, 0);
}

/**
 * @brief  Y轴移动(前后, 电机1+2), 非阻塞
 * @param  dir   方向: 0=后, 1=前
 * @param  vel   速度(RPM)
 * @param  acc   加速度
 * @param  distance 距离(mm)
 */
void Motor_Y(uint8_t dir, uint16_t vel, uint8_t acc, float distance)
{
    uint32_t clk = distancetoclk(distance);
    Motorflag = 0;
    Emm_V5_Pos_Control(1, dir, vel, acc, clk, 0, 1);
    Emm_V5_Pos_Control(2, dir, vel, acc, clk, 0, true);
    Emm_V5_Synchronous_motion(0);
}

/**
 * @brief  Z轴移动(上下升降, 电机4), 非阻塞
 * @param  dir   方向: 0=上, 1=下
 * @param  vel   速度(RPM)
 * @param  acc   加速度
 * @param  distance 距离(mm)
 */
void Motor_Z(uint8_t dir, uint16_t vel, uint8_t acc, float distance)
{
    uint32_t clk = (distance / Gear(24)) * 3200 * 14;
    z_flag = 0;
    z_ack = 0;
    Emm_V5_Pos(4, dir, vel, acc, clk, 0, 0);
}

/**
 * @brief  XZ轴同时移动(左右+上下), 阻塞, 以Z轴到位为完成判断
 *         使用应答机制确保每条指令都被驱动器收到
 */
void Motor_XY(uint8_t x_dir, uint16_t x_vel, uint8_t x_acc, float x_distance,
               uint8_t y_dir, uint16_t y_vel, uint8_t y_acc, float y_distance)
{
    uint32_t x_clk = (x_distance / Gear(72)) * 320 * 37;


    x_flag = 0;

    x_ack = 0;



    /* Z轴确认收到, 再发X轴 */
    SendAndWait(3, x_dir, x_vel, x_acc, x_clk, 3);
Motor_Y(y_dir, y_vel, y_acc, y_distance);
    /* 等待Z轴到位作为完成判断 */
  uint32_t wait_start = HAL_GetTick();
    const uint32_t TIME_OUT_MS = 7500;

    while (1)
    {
        // X、Y全部到位，正常退出
        if (x_flag && Motorflag)
        {
            break;
        }
        // 3秒超时强制跳出并上报超时帧
        if (HAL_GetTick() - wait_start >= TIME_OUT_MS)
        {

            break;
        }
        HAL_Delay(5);
    }

    x_flag = 0;
    Motorflag = 0;
}
void Motor_XZ(uint8_t x_dir, uint16_t x_vel, uint8_t x_acc, float x_distance,
               uint8_t z_dir, uint16_t z_vel, uint8_t z_acc, float z_distance)
{
    uint32_t x_clk = (x_distance / Gear(72)) * 320 * 37;
    uint32_t z_clk = (z_distance / Gear(24)) * 3200 * 14;

    x_flag = 0;
    z_flag = 0;
    x_ack = 0;
    z_ack = 0;

    /* 先发Z轴, 等命令收到应答 */
    SendAndWait(4, z_dir, z_vel, z_acc, z_clk, 3);
    /* Z轴确认收到, 再发X轴 */
    SendAndWait(3, x_dir, x_vel, x_acc, x_clk, 3);
uint32_t wait_start = HAL_GetTick();
    const uint32_t TIME_OUT_MS = 5000;
    /* 等待Z轴到位作为完成判断 */
  while (1)
    {
        // X、Y全部到位，正常退出
        if (x_flag && z_flag)
        {
            break;
        }
        // 3秒超时强制跳出并上报超时帧
        if (HAL_GetTick() - wait_start >= TIME_OUT_MS)
        {

            break;
        }
        HAL_Delay(5);
    }
    x_flag = 0;
    z_flag = 0;
}
void Motor_YZ(uint8_t y_dir, uint16_t y_vel, uint8_t y_acc, float y_distance,
               uint8_t z_dir, uint16_t z_vel, uint8_t z_acc, float z_distance)
{

    uint32_t z_clk = (z_distance / Gear(24)) * 3200 * 14;

    z_flag = 0;
    z_ack = 0;

    /* 先发Z轴, 等命令收到应答 */
    SendAndWait(4, z_dir, z_vel, z_acc, z_clk, 3);
 Motor_Y(y_dir, y_vel, y_acc, y_distance);


    /* 等待Z轴到位作为完成判断 */
    while (z_flag == 0||Motorflag == 0);
	
    Motorflag = 0;
    z_flag = 0;
}
/**
 * @brief  XYZ三轴移动, 阻塞, 以Z轴到位为完成判断
 *         使用应答机制确保每条指令都被驱动器收到
 * @param  x_dir   X轴方向: 0=左, 1=右
 * @param  y_dir   Y轴方向: 0=后, 1=前
 * @param  z_dir   Z轴方向: 0=上, 1=下
 */
void Motor_XYZ(uint8_t x_dir, uint16_t x_vel, uint8_t x_acc, float x_distance,
                uint8_t y_dir, uint16_t y_vel, uint8_t y_acc, float y_distance,
                uint8_t z_dir, uint16_t z_vel, uint8_t z_acc, float z_distance)
{
    uint32_t x_clk = (x_distance / Gear(72)) * 320 * 37;		//发送脉冲数量（37:1）
    uint32_t z_clk = (z_distance / Gear(24)) * 3200 * 14;		//发送脉冲数量（14:1）

    x_flag = 0;
    z_flag = 0;
    x_ack = 0;
    z_ack = 0;

    /* 先发Z轴, 等命令收到应答 */
    SendAndWait(4, z_dir, z_vel, z_acc, z_clk, 3);
	
    /* Z轴确认收到, 再发X轴 */
    SendAndWait(3, x_dir, x_vel, x_acc, x_clk, 3);
    /* Y轴(前后)用huart2, 不存在冲突问题 */
    Motor_Y(y_dir, y_vel, y_acc, y_distance);


  

    /* 等待三轴都到位，带5s超时 */
    while (1)
    {
        // 三轴全部到位，正常退出等待
        if (x_flag == 1 && z_flag == 1 && Motorflag == 1)
        {
            break;
        }

    }
    Motorflag = 0;
    x_flag = 0;
    z_flag = 0;
}

void Motor_XYZ6(uint8_t x_dir, uint16_t x_vel, uint8_t x_acc, float x_distance,
                uint8_t y_dir, uint16_t y_vel, uint8_t y_acc, float y_distance,
                uint8_t z_dir, uint16_t z_vel, uint8_t z_acc, float z_distance,
                uint32_t wait_timeout_ms)
{
    uint32_t x_clk = (x_distance / Gear(72)) * 320 * 37;		//发送脉冲数量（37:1）
    uint32_t z_clk = (z_distance / Gear(24)) * 3200 * 14;		//发送脉冲数量（14:1）

    x_flag = 0;
    z_flag = 0;
    x_ack = 0;
    z_ack = 0;

    /* 先发Z轴, 等命令收到应答 */
    SendAndWait(4, z_dir, z_vel, z_acc, z_clk, 3);
    /* Z轴确认收到, 再发X轴 */
    SendAndWait(3, x_dir, x_vel, x_acc, x_clk, 3);
    /* Y轴(前后)用huart2, 不存在冲突问题 */
    Motor_Y(y_dir, y_vel, y_acc, y_distance);

    // 记录等待起始时间，使用外部传入的超时时间
    uint32_t wait_start = HAL_GetTick();

    // 带超时等待三轴到位
    while (1)
    {
        // 三轴全部到位，正常退出
        if (x_flag && z_flag && Motorflag)
        {
            break;
        }
        // 超过设定超时无全部到位信号，强制跳出循环并上报超时
        if (HAL_GetTick() - wait_start >= wait_timeout_ms)
        {
            // 超时上报帧 number_position[5]，0x02标记超时
            uint8_t number_position[5] = {0,0,0,0x02,0};
            HAL_UART_Transmit(&huart1, number_position, 5, 10);
            break;
        }
        // 小幅延时，降低CPU占用
        HAL_Delay(5);
    }

    Motorflag = 0;
    x_flag = 0;
    z_flag = 0;
}
void Motor_GetPosition(uint8_t addr)
{
	uint8_t cmd[3]={addr,0x36,0x6B};

	__HAL_UART_FLUSH_DRREGISTER(&huart2);
		HAL_UART_DMAStop(&huart2);

		HAL_UART_Receive_DMA(&huart2,Position_Rx, 8);
    HAL_Delay (300);
    HAL_UART_Transmit_DMA(&huart2, cmd, 3);
	  HAL_Delay (20);

	if(Position_Rx[0]==addr&&Position_Rx[1]==0x36)
	{
		for(int i=3;i<7;i++)
		{
			sum_position=(sum_position<<8)|Position_Rx[i];
		}
//		if(Position_Rx[2]==0x01)
//		{
//			sum_position=-sum_position;
//		}
	}
	__HAL_UART_FLUSH_DRREGISTER(&huart2);
		HAL_UART_DMAStop(&huart2);
	HAL_UART_Receive_DMA(&huart2, Motor_Rx, 4);
	move_distance=(((float)sum_position)/65536)*Gear(55);
		  Motorflag = 0;

}

