#ifndef __MOTOR_ACTION_H
#define __MOTOR_ACTION_H


extern  uint8_t flag_1;
extern  uint8_t flag_2;
void Action_1(void);
void Action_2(void);
void Action_3(void);
void Action_4(void);
void Action_5(void);
void Action_6(void);
void Action_7(void);
void Action_8(void);
void Action_9(void);
void Action_10(void);
void frame_1(void);
void frame_2(void);
void frame_3(void);
void bean_action_G1(void);
void bean_action_Y1(void);
void bean_action_W1(void);
void bean_action_G2_1(void);
void bean_action_G2_2(void);
void bean_action_W2_1(void);
void bean_action_W2_2(void);
void bean_action_Y2_1(void);
void bean_action_Y2_2(void);
void bean_action_G3(void);
void bean_action_W3(void);
void bean_action_Y3(void);
void Data_Handles(uint8_t key, uint8_t mode, uint8_t *pos_array);
void task(uint8_t c1, uint8_t c2, uint8_t c3, uint8_t *pos_array);
#endif
