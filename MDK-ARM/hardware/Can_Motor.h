#ifndef __CAN_MOTOR_H
#define __CAN_MOTOR_H

HAL_StatusTypeDef CAN_Send_MotorCurrent(uint8_t motor_id, int16_t current);
void CAN_Motor_Init(void);














#endif
