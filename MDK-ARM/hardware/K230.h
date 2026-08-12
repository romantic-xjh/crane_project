#ifndef __K230_H
#define __K230_H
#include "stm32f4xx_hal.h"
extern	__IO uint8_t Bomb_number;
extern	__IO uint8_t Target_number;
extern	__IO uint8_t Save_number;
extern __IO uint16_t center_x;
extern __IO uint16_t center_y;
extern uint16_t X_Adiust;
extern uint16_t Y_Adiust;
extern uint16_t X_ShootAdiust;
extern uint16_t Y_ShootAdiust;
extern uint16_t X_SaveAdiust;
extern uint16_t Y_SaveAdiust;
extern uint8_t K230_Rx[8];
extern uint8_t K230_Rxflag;



extern __IO unsigned char target;
extern uint8_t Command_Data[8];
void K230_DMA_Init(void);
void Bean_Show(void);
void Data_Handle1(uint8_t key);
void K230_Command(uint8_t command);
void Data_Handle1_1(uint8_t key);

#endif

