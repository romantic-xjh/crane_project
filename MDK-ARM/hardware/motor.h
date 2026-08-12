#ifndef __MOTOR_H_
#define __MOTOR_H_


#include "stm32f4xx_hal.h"              // Device header
#include "Emm_V5.h"
extern __IO uint8_t x_flag;
extern __IO uint8_t z_flag;
extern __IO uint8_t x_ack;
extern __IO uint8_t z_ack; 
extern float move_distance;
extern int32_t sum_position;
extern uint8_t Position_Rx[8];
int distancetoclk(int distance);
void Motor_XYZ(uint8_t x_dir, uint16_t x_vel, uint8_t x_acc, float x_distance,
	             uint8_t y_dir, uint16_t y_vel, uint8_t y_acc, float y_distance,
               uint8_t z_dir, uint16_t z_vel, uint8_t z_acc, float z_distance);//X轴方向: 0=左, 1=右,y轴方向:0=后, 1=前;Z轴方向: 0=上, 1=下
void Motor_XYZ6(uint8_t x_dir, uint16_t x_vel, uint8_t x_acc, float x_distance,
                uint8_t y_dir, uint16_t y_vel, uint8_t y_acc, float y_distance,
                uint8_t z_dir, uint16_t z_vel, uint8_t z_acc, float z_distance,
                uint32_t wait_timeout_ms);

void Forward(int step,double v);
void Backward(int step,int v);
void Left(int step,int v);
void Right(int step,int v);
void ForLeft(int step,int v);
void ForRight(int step,int v);
void BackLeft(int step,int v);
void BackRight(int step,int v);
void Up(int step,int v);
void Down(int step,int v);

void Uart_ForRight( uint16_t vel, uint8_t acc, float distance);
void Uart_Forward(uint16_t vel, uint8_t acc, float distance, uint32_t wait_timeout_ms);
void Uart_ForLeft( uint16_t vel, uint8_t acc, float distance);
void Uart_Backward(uint16_t vel, uint8_t acc, float distance, uint32_t wait_timeout_ms);
void Uart_BackLeft( uint16_t vel, uint8_t acc, float distance);
void Uart_BackRight( uint16_t vel, uint8_t acc, float distance);
void Uart_Left(uint16_t vel, uint8_t acc, float distance, uint32_t wait_timeout_ms);
void Uart_Right(uint16_t vel, uint8_t acc, float distance, uint32_t wait_timeout_ms);
void Uart_TurnRight( uint16_t vel, uint8_t acc, uint32_t clk);
void Uart_TurnLeft( uint16_t vel, uint8_t acc, uint32_t clk);
void Uart_Up( uint16_t vel, uint8_t acc, float distance);
void Uart_Down( uint16_t vel, uint8_t acc, float distance);

// XYZ非阻塞移动函数
void Motor_X(uint8_t dir, uint16_t vel, uint8_t acc, float distance);
void Motor_Y(uint8_t dir, uint16_t vel, uint8_t acc, float distance);
void Motor_Z(uint8_t dir, uint16_t vel, uint8_t acc, float distance);
void Motor_XZ(uint8_t x_dir, uint16_t x_vel, uint8_t x_acc, float x_distance,
               uint8_t z_dir, uint16_t z_vel, uint8_t z_acc, float z_distance);
void Motor_XY(uint8_t x_dir, uint16_t x_vel, uint8_t x_acc, float x_distance,
               uint8_t y_dir, uint16_t y_vel, uint8_t y_acc, float y_distance);
void Motor_YZ(uint8_t y_dir, uint16_t y_vel, uint8_t y_acc, float y_distance,
               uint8_t z_dir, uint16_t z_vel, uint8_t z_acc, float z_distance);

//void Uart_TurnLefttest180( uint16_t vel, uint8_t acc);
void Uart_Turn180(void);
void Vehicle_Rotate(int TargetAngle,float Accuracy,float vel);
void Turn (int Target,float Accuracy,float vel, float acc);
void Motor_VelStraight(uint8_t Speed,uint8_t acc);
void Motor_VelTurnBack(uint8_t Speed,uint8_t acc);
void Motor_Velturnleft(uint8_t Speed,uint8_t acc);
void Motor_Velturnright(uint8_t Speed,uint8_t acc);
void Motor_SetStop(uint8_t adr);
//void Vision_Adjust(int dx,int dy);

extern uint8_t Motor_Rx[4];
void DMA_motor_Init(void);
void Motor_AngleAdjust(float target_angle, float error_range);
void Motor_GetPosition(uint8_t addr);
#endif


