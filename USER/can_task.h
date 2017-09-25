#ifndef _CAN_TASK_H_
#define _CAN_TASK_H_

#include "includes.h"




#define CAN_SAE_PRIO							7
#define CAN_SAE_STK_SIZE					128


#define VAILD 	1
#define INVAILD 0

extern OS_STK CAN_SAE_TASK_STK[CAN_SAE_STK_SIZE];

void can_task_init(void);
void can_sae_task(void *pdata);
void resolve_can_message(CanRxMsg *msg);

#endif


