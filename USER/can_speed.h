#ifndef _CAN_SPEED_H_
#define _CAN_SPEED_H_

#include "stm32f10x.h"

extern volatile u8 g_speed;

void uart_send_speed(u8 speed);

void can_send_speed(u8 speed);

void can_resolve_speed(u8 *buf,u8 idex);

s8 can_obdii_resolve_speed(u8 *buf);

s8 obdii_requeset(int pid);

u8 get_can_speed(void);

#endif
