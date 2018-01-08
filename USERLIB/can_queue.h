#ifndef _CAN_QUEUE_H
#define _CAN_QUEUE_H

#include "can.h"
#include "Encode.h"

#define CAN_QUEUE_SIZE 8
#define SEND_QUEUE_SIZE 8

typedef struct _queue_info{
	uint8_t 	 size;
	uint8_t 	 get;
	uint8_t 	 put;
	uint8_t 	 mark;
	CanRxMsg     *rxbuf;
	UartRxMsg      *txbuf;
}QUEUE_INFO;


void    can_queue_init(QUEUE_INFO *info,uint8_t size);

void    put_data_queue(QUEUE_INFO *info,CanRxMsg *msg);
uint8_t get_data_queue(QUEUE_INFO *info,CanRxMsg *msg);
uint8_t is_queue_Full(QUEUE_INFO *info);

void    can_sendqueue_init(QUEUE_INFO *info,uint8_t size);
void    put_send_queue(QUEUE_INFO *info,uint8_t *msg);
uint8_t is_sendqueue_Full(QUEUE_INFO *info);
uint8_t get_send_queue(QUEUE_INFO *info,UartRxMsg *msg);


extern QUEUE_INFO CAN_QUEUE_INFO;
extern QUEUE_INFO CAN_SEND_INFO;

#endif
