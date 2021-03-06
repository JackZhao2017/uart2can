#include <stdio.h>
#include "stm32f10x.h"
#include "led.h"
#include "delay.h"
#include "usart.h"
#include "can.h"
#include "string.h"
#include "can_speed.h"
#include "crc8.h"

//#define printf

#define factor 1/256
#define SPEED_FACTOR  0.05625
static 	 u8 buf[]={0x55,0x30,0x6,0x8,0x55,0xc,0x09,0x46};
static 	 u8 request_buf[]={0x02, 0x01 ,0x0D, 0x00, 0x00, 0x00, 0x00, 0x00};
static u16 g_speed=60;

static u8 g_isvaild=INVAILD;

void SPEED_SetCanBusStatus(u8 mode)
{
	  g_isvaild=mode;
}
u8 SPEED_GetCanBusStatus(void)
{
	return g_isvaild;
}

u16 SPEED_GetCanSpeed(void)
{
	return g_speed;
}

void SPEED_SAE1939_Resolve(u8 *buf,u8 idex)
{
	g_speed=buf[idex];
}

s8 SPEED_OBDII_Resolve(u8 *buf)
{
	u8 datalen=buf[0];
	u8 mode =buf[1];
	u8 pid	=buf[2];

	if(mode!=0x41){
		return -1;
	}
	if(pid!=0x0d){
		return -1;
	}
	g_speed=buf[datalen];

	return 0;
}
s8 SPEED_OBDII_Requeset(int pid)
{
	switch(pid)
	{
		case OBDII_SPEED_PID:
			break;
		default:
			return -1;
	}

	request_buf[0]=0x02;
	request_buf[1]=0x01;
	request_buf[2]=pid;
	can_tx(OBDII_CMD_ID,CAN_ID_STD,request_buf,8);	 
	return 0;
}

void SPEED_CanSend(u16 speed)
{
	can_tx(0x0cfe6c07,CAN_ID_EXT,buf,8);
}

void SPEED_UartSend(u16 speed)
{
	u8 i=0;
	buf[0]=0x55;
	buf[1]=0x04;
	buf[2]=(speed>>8)&0xff;
	buf[3]= speed&0xff;
	buf[4]= crc8_creator(buf,1,3);
	for(i=0;i<5;i++){
		USART2->DR=buf[i];
		while((USART2->SR&0X40)==0);
	}
}



