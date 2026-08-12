#include "stm32f4xx.h"
#include "LobotServoController.h"
#include "servo.h"

/*********************************************************************************
 * Function:  Servo_Open
 * Description： 打开阀门
 * Parameters:   times: 执行次数（0=无限循环）
 * Return:       无
 * Others:
 **********************************************************************************/
void Servo_Open(uint16_t times)
{
    runActionGroup(SERVO_ACTION_OPEN, times);
}

/*********************************************************************************
 * Function:  Servo_Close
 * Description： 关闭阀门
 * Parameters:   times: 执行次数（0=无限循环）
 * Return:       无
 * Others:
 **********************************************************************************/
void Servo_Close(uint16_t times)
{
    runActionGroup(SERVO_ACTION_CLOSE, times);
}

/*********************************************************************************
 * Function:  Servo_Rotate90
 * Description： 旋转90度
 * Parameters:   times: 执行次数（0=无限循环）
 * Return:       无
 * Others:
 **********************************************************************************/
void Servo_Rotate90(uint16_t times)
{
    runActionGroup(SERVO_ACTION_ROTATE_90, times);
}


/*********************************************************************************
 * Function:  Servo_Reset
 * Description： 转正
 * Parameters:   times: 执行次数（0=无限循环）
 * Return:       无
 * Others:
 **********************************************************************************/
void Servo_Reset(uint16_t times)
{
    runActionGroup(SERVO_ACTION_RESET, times);
}

/*********************************************************************************
 * Function:  Servo_RunAction
 * Description： 通用舵机动作执行
 * Parameters:   action: 动作组编号（SERVO_ACTION_OPEN/CLOSE/ROTATE_90/RESET）
 *               times:  执行次数（0=无限循环）
 * Return:       无
 * Others:
 **********************************************************************************/
void Servo_RunAction(uint8_t action, uint16_t times)
{
    runActionGroup(action, times);
}

/*********************************************************************************
 * Function:  Serio_face
 * Description： 保持正向
 * Parameters:   
 *               times:  执行次数（0=无限循环）
 * Return:       无
 * Others:
 **********************************************************************************/

void Serio_face(uint16_t times)
{
	runActionGroup(SERVO_ACTION_FACE, times);

}
