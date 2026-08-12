#ifndef __SERVO_H_
#define __SERVO_H_

#include "stm32f4xx_hal.h"

// 舵机动作组编号定义
#define SERVO_ACTION_OPEN       0    // 打开（阀门张开）
#define SERVO_ACTION_CLOSE      1    // 关闭（阀门闭合）
#define SERVO_ACTION_FACE 			2		 // 保持正向
#define SERVO_ACTION_ROTATE_90  3    // 旋转90度
#define SERVO_ACTION_RESET      4    // 回正

// 舵机动作封装函数
void Servo_Open(uint16_t times);           // 打开夹爪
void Servo_Close(uint16_t times);          // 关闭夹爪
void Servo_Rotate90(uint16_t times);       // 旋转90度
void Servo_Reset(uint16_t times);          // 回正
void Servo_RunAction(uint8_t action, uint16_t times);  // 通用动作执行
void Serio_face(uint16_t times);

#endif
