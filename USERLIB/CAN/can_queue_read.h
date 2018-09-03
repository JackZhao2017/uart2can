#ifndef _CAN_QUEUE_READ_H
#define _CAN_QUEUE_READ_H

#include "can.h"

typedef struct _queue_read_info{
	uint8_t 	 size;
	uint8_t 	 get;
	uint8_t 	 put;
	uint8_t 	 mark;
	CanRxMsg   *rxbuf;
}QUEUE_READ_INFO;


#define READ_QUEUE_SIZE 8

void    CAN_QueueReadInit(QUEUE_READ_INFO *info,int size);

void    CAN_QueueReadPut(QUEUE_READ_INFO *info,CanRxMsg *msg);
uint8_t CAN_QueueReadGet(QUEUE_READ_INFO *info,CanRxMsg *msg);
uint8_t CAN_QueueReadIsFull(QUEUE_READ_INFO *info);
void    CAN_QueueReadDelHead(QUEUE_READ_INFO *info);
uint8_t CAN_QueueReadGetHead(QUEUE_READ_INFO *info,CanRxMsg **msg);
void    CAN_QueueReadClean(QUEUE_READ_INFO *info);


#endif
