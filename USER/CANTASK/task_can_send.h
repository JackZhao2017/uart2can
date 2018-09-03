#ifndef _CAN_TASK_SEND_H_
#define _CAN_TASK_SEND_H_


void 	CAN_SendTaskInit(void);
uint8_t CAN_SendTaskStart(void);
uint8_t CAN_SendTaskPause(void);
uint8_t CAN_GetSendTaskStatus(void);

void    CAN_SendOneFrame(u8 *msg);





#endif


