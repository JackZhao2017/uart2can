#include "detect.h"
#include "ringbuf.h"
#include "crc8.h"
#include "Encode.h"
#include "control.h"
#include <stdio.h>
#include <stdint.h>

#define u8  unsigned char 

unsigned char  addringaddr(RINGBUFFER *info,int inc)
{
	unsigned char  addr=info->getaddr;
	return ((addr+inc)<info->size)?(addr+inc):(addr+inc-info->size);
}

char detectvaildSync(RINGBUFFER *info,unsigned char sync)
{
	if(info->num-info->getnum<=0)
		return 0;
	while(info->num-info->getnum>1){
		
		if(info->data[info->getaddr]==sync){
			info->getaddr=addringaddr(info,1);
			info->getnum+=1;	
			return 1;
		}else{
			info->getaddr=addringaddr(info,1);
			info->getnum+=1;			
		}
	}
	return 0;
}

char detectvaildMsgtype(RINGBUFFER *info,unsigned char *type)
{
	unsigned char  id=0;
	signed char ret=0;
	if(info->num-info->getnum<2)
		return 0;
	id=info->data[info->getaddr];
	switch(id)
	{
		case CANMSG:
		case CMDMSG:				
			 *type=id;
			 ret=1;
			 break;
		default:
			 printf("No vaild type %d\r\n",id);
			 ret=-1;
		break;
	}
	return ret;
}

char detectcrc8(unsigned char *msg,u8 start,u8 len)
{
	u8 crc=crc8_creator(msg,start,len);
	if(crc==msg[len]){
		return 1;
	}
	printf("crc not right %d %d\r\n",crc,msg[len]);
	return 0;
}

