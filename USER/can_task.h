#ifndef _CAN_TASK_H_
#define _CAN_TASK_H_

#include "includes.h"
#include "Encode.h"
#include "can.h"



void can_task_init(BAUDRATE brandrate);
void can_sae_task(void *pdata);
void resolve_can_message(CanRxMsg *msg);

void pause_can_task(void);
void start_canread_task(void);
void start_cansend_task(void);
void can_set_brandrate(BAUDRATE brandrate);
void can_set_filter(FILTER *filter,uint8_t len);

uint8_t getCanMsg(CanRxMsg *msg);
void putcansendPacket(unsigned char *buf);
void startcansend(void);

#endif


