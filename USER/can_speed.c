#include "stdio.h"
#include "stm32f10x.h"
#include "led.h"
#include "delay.h"
#include "usart.h"
#include "can.h"
#include "string.h"
#include "can_speed.h"

#define factor 1/256
#define SPEED_FACTOR  0.05625
static 	 u8 buf[]={0x55,0x30,0x6,0x8,0x55,0xc,0x0};
static 	 u8 request_buf[]={0x02, 0x01 ,0x0D, 0x00, 0x00, 0x00, 0x00, 0x00};
volatile u8 g_speed=0;

u8 get_can_speed(void)
{
		return g_speed;
}

void can_resolve_speed(u8 *buf,u8 idex)
{
		u8 speed;
		speed=buf[idex];
		g_speed=speed;
		printf(" g_speed =%d \r\n",g_speed);
}

s8 can_obdii_resolve_speed(u8 *buf)
{
	  u8 datalen=buf[0];
		u8 mode   =buf[1];
		u8 pid		=buf[2];
	
		if(mode!=0x41){
			 return -1;
		}
		if(pid!=0x0d){
			 return -1;
		}
		g_speed=buf[datalen];
		printf("obdii speed =%d \r\n",g_speed);
		
		return 0;
}
s8 obdii_requeset(int pid)
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
void can_send_speed(u8 speed)
{
		float fspeed;
		u16 t=0;
	  fspeed=(float)speed/SPEED_FACTOR;
		printf("can_send_speed %f\r\n",fspeed);
    t=(u16)fspeed;
    buf[1]=(t>>5)&0xff;
    buf[2]=(t&0x1f)<<3;
		can_tx(0x125,CAN_ID_STD,buf,8);
}
void uart_send_speed(u8 speed)
{
		USART2->DR=speed&0xff;
		while((USART2->SR&0X40)==0);
		printf("uart_send_speed %d\r\n",speed);
}



