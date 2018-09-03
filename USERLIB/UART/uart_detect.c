#include "uart_detect.h"
#include "crc8.h"
#include "control.h"
#include <stdio.h>
#include <stdint.h>

#define u8  unsigned char 


u8  UART_DetectvaildSync(unsigned char *buf,unsigned char index)
{
	if(buf==NULL)
		return 0;
	
	if(buf[index]==0x55){	
		
		return 1;
	}
	printf("%s %x\r\n",__func__,buf[index] );	
	return 0;
}

u8 UART_DetectvaildMsgtype(unsigned char *buf,unsigned char *type)
{
	u8 ret=0;
	if(buf==NULL)
		return 0;
	*type=buf[0];
	switch(*type){
		case CANMSG:
		case CMDSETMSG:		
		case CMDIDMSG:			 	
			 ret=1;
			 break;
		default:
			 printf("%s type %x\r\n",__func__,*type);
			 ret=0;
			 break;
	}
	return ret;
}
u8 UART_Detectcrc8(unsigned char *msg,unsigned char  start, unsigned char  len)
{
	u8 crc=crc8_detector(msg,start,len);
	if(crc==msg[len]){
		 return 1;
	}
	printf("crc not right %d %d %d \r\n",crc,msg[len],len);
	return 0;
}

