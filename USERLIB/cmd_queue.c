#include "cmd_queue.h"
#include "usart2.h"
#include "task_can.h"
#include "includes.h"
#include "task_config.h"
#include "control.h"


uint8_t cmd[CMDQUEUESIZE][CMDSIZE];


void cmd_queue_init(CMD_QUEUE_INFO *info,uint8_t size)
{
	char i=0;
	memset(info,0,sizeof(CMD_QUEUE_INFO));
	info->mark=size-1;
	info->size=size;
	for(i=0;i<CMDQUEUESIZE;i++){
		info->buf[i]=&cmd[i][0];
	}
	printf("%s  %d \r\n",__func__ ,sizeof(cmd[0]));
}

char put_cmd_queue(CMD_QUEUE_INFO *info,uint8_t *data)
{
	if(((info->put+1)&info->mark)==info->get){
		printf("%s full \r\n",__func__);
		return 0;
	}
	memcpy(info->buf[info->put],data,data[0]);
	info->put=(info->put+1)&info->mark;
	return 1;
}
char get_cmd_queue(CMD_QUEUE_INFO *info,uint8_t *data)
{
	if(info->get==info->put)
		return 0;
	memcpy(data,info->buf[info->get],CMDSIZE);
	info->get=(info->get+1)&info->mark;
	return 1;
}

void clean_cmd_queue(CMD_QUEUE_INFO *info)
{
	info->get=0;
	info->put=0;
}

