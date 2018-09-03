#ifndef _CMD_QUEUE_H_
#define _CMD_QUEUE_H_

#include <stdint.h>

#define  CMDQUEUESIZE 8




typedef struct {
	uint8_t 	 size;
	uint8_t 	 get;
	uint8_t 	 put;
	uint8_t 	 mark;
	uint8_t      *buf[CMDQUEUESIZE];
}CMD_QUEUE_INFO;


void cmd_queue_init(CMD_QUEUE_INFO *info,uint8_t size);
char put_cmd_queue(CMD_QUEUE_INFO *info,uint8_t *data);
char get_cmd_queue(CMD_QUEUE_INFO *info,uint8_t *data);
void clean_cmd_queue(CMD_QUEUE_INFO *info);
#endif
