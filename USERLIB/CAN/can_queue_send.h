#ifndef _CAN_QUEUE_SEND_H
#define _CAN_QUEUE_SEND_H

#include "can.h"
#include "uart_encode.h"

#define SEND_QUEUE_SIZE 8

typedef struct _queue_send_info{
	uint8_t 	 size;
	uint8_t 	 get;
	uint8_t 	 put;
	uint8_t 	 mark;
	UartRxMsg  *txbuf;
}QUEUE_SEND_INFO;


void 	 CAN_QueueSendInit(QUEUE_SEND_INFO *info,int size);
void 	 CAN_QueueSendPut(QUEUE_SEND_INFO *info,unsigned char  *msg);
uint8_t  CAN_QueueSendGet(QUEUE_SEND_INFO *info,UartRxMsg *msg);
void 	 CAN_QueueSendClean(QUEUE_SEND_INFO *info);
uint8_t  CAN_QueueSendIsFull(QUEUE_SEND_INFO *info);





#endif
