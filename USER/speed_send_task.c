/*	Created by Jack 2017/9/6 */
#include "includes.h"
#include "taskconfig.h"
#include "can_speed.h"


void speed_send_task(OS_TMR *ptmr, void *pdata)
{
		 u8 speed =get_can_speed();	 
		 //can_send_speed(speed);
		 uart_send_speed(speed);
}

