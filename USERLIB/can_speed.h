#ifndef _CAN_SPEED_H_
#define _CAN_SPEED_H_

#include "stm32f10x.h"


#define VAILD 	1
#define INVAILD 0

void uart_send_speed(u16 speed);

void can_send_speed(u16 speed);

void can_resolve_speed(u8 *buf,u8 idex);

s8 can_obdii_resolve_speed(u8 *buf);

s8 obdii_requeset(int pid);

u16 get_can_speed(void);

void set_canbus_status(u8 mode);
u8   get_canbus_status(void);

#endif
