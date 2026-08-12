#include "can.h"
#include <string.h>
#include "main.h"

#define CAN_MOTOR_ID_BASE       0x200       /* 控制帧基 ID */
#define CAN_MOTOR_MAX_NUM       2           /* 最大电机数量 */

typedef struct
{
    int16_t  angle;          /* 编码器角度（0~8191 对应 0~360°） */
    int16_t  speed_rpm;      /* 当前转速（RPM） */
    int16_t  current_raw;    /* 当前电流（原始值） */
} CAN_Motor_FB_t;

CAN_Motor_FB_t g_MotorFB[CAN_MOTOR_MAX_NUM];
/* ================================================================
 *  发送函数：给指定编号的无刷电机发送控制指令
 *  motor_id : 电机编号 1~2
 *  current  : 目标电流，范围 -10000 ~ 10000（对应 -10A ~ +10A，单位 mA）
 *  返回值   : HAL_OK / HAL_ERROR
 * ================================================================ */
HAL_StatusTypeDef CAN_Send_MotorCurrent(uint8_t motor_id, int16_t current)
{
      CAN_TxHeaderTypeDef   TxHeader;
      uint32_t              TxMailbox;
      uint8_t               TxData[8];


      /* ---------- 构造 CAN 发送帧 ---------- */
      TxHeader.StdId    = 0x200;     
      TxHeader.ExtId    = 0x00;															 /* 扩展 ID: 不用都为0 */
      TxHeader.IDE      = CAN_ID_STD;                        /* 标准帧（11 位 ID） */
      TxHeader.RTR      = CAN_RTR_DATA;                      /* 数据帧（非远程帧） */
      TxHeader.DLC      = 8;                                 /* 数据长度 = 8 字节 */
      TxHeader.TransmitGlobalTime = DISABLE;

      /* ---------- 填充 8 字节数据 ---------- */
      /* 字节 0~1: 电机 1 电流值（高字节在前 / 大端） */
      TxData[0] = (current >> 8) & 0xFF;                     /* 电流高字节 */
      TxData[1] = current & 0xFF;                            /* 电流低字节 */

      /* 字节 2~3: 电机 2 电流值（若一个 CAN 帧控制多个电机） */
      TxData[2] = (current >> 8) & 0xFF;;
      TxData[3] = current & 0xFF;

      /* 字节 4~5: 电机 3 电流值 */
      TxData[4] = 0x00;
      TxData[5] = 0x00;

      /* 字节 6~7: 电机 4 电流值 */
      TxData[6] = 0x00;
      TxData[7] = 0x00;

      /* ---------- 发送到 CAN 总线 ---------- */
      return HAL_CAN_AddTxMessage(&hcan1, &TxHeader, TxData, &TxMailbox);
}

//配置无刷电机发送-过滤器
static void CAN_Filter_Config_MaskMode(void)
 {
      CAN_FilterTypeDef sFilterConfig;
      sFilterConfig.FilterActivation   = CAN_FILTER_ENABLE;
      sFilterConfig.FilterBank         = 0;
      sFilterConfig.FilterMode         = CAN_FILTERMODE_IDLIST;
      sFilterConfig.FilterScale        = CAN_FILTERSCALE_32BIT;
      sFilterConfig.FilterFIFOAssignment = CAN_FILTER_FIFO0;
      sFilterConfig.FilterIdHigh       = 0x200 << 5;
      sFilterConfig.FilterIdLow        = 0x0000;
      sFilterConfig.FilterMaskIdHigh   = 0x201 << 5;
      sFilterConfig.FilterMaskIdLow    = 0x0000;
      HAL_CAN_ConfigFilter(&hcan1, &sFilterConfig);
 }

 
void CAN_Motor_Init(void)
  {
      /* 1. 配置过滤器 */
      CAN_Filter_Config_MaskMode();               

      /* 2. 启动 CAN 外设 */
      HAL_CAN_Start(&hcan1);

      /* 3. 使能 FIFO0 接收中断 */
      HAL_CAN_ActivateNotification(&hcan1, CAN_IT_RX_FIFO0_MSG_PENDING);

      /* 4. 初始化电机反馈缓存 */
      memset(g_MotorFB, 0, sizeof(g_MotorFB));
  }

	
	
//中断回调（在接收到数据时回调）
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
  {
      CAN_RxHeaderTypeDef   RxHeader;
      uint8_t               RxData[8];

      HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &RxHeader, RxData);

      /* 根据 ID 直接判断是哪台电机的反馈，跟先后顺序无关 */
      if (RxHeader.StdId == 0x201)          /* ID = 电机1 */
      {
          g_MotorFB[0].angle     = (RxData[0] << 8) | RxData[1];
          g_MotorFB[0].speed_rpm = (RxData[2] << 8) | RxData[3];
          g_MotorFB[0].current_raw   = (RxData[4] << 8) | RxData[5];
      }
      else if (RxHeader.StdId == 0x202)     /* ID = 电机2 */
      {
          g_MotorFB[1].angle     = (RxData[0] << 8) | RxData[1];
          g_MotorFB[1].speed_rpm = (RxData[2] << 8) | RxData[3];
          g_MotorFB[1].current_raw   = (RxData[4] << 8) | RxData[5];         
      }

      HAL_CAN_ActivateNotification(hcan, CAN_IT_RX_FIFO0_MSG_PENDING);
  }