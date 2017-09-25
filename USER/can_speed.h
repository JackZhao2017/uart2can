#ifndef _CAN_SPEED_H_
#define _CAN_SPEED_H_

#include "stm32f10x.h"

extern volatile u16 g_speed;

void uart_send_speed(u16 speed);

void can_send_speed(u16 speed);

void can_resolve_speed(u8 *buf,u8 idex);

s8 can_obdii_resolve_speed(u8 *buf);

s8 obdii_requeset(int pid);

u16 get_can_speed(void);

#endif
