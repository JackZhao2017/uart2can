#ifndef _CAN_SPEED_H_
#define _CAN_SPEED_H_

#include "stm32f10x.h"


#define VAILD 	1
#define INVAILD 0

void SPEED_UartSend(u16 speed);
void SPEED_CanSend(u16 speed);
void SPEED_SAE1939_Resolve(u8 *buf,u8 idex);
s8   SPEED_OBDII_Resolve(u8 *buf);
s8   SPEED_OBDII_Requeset(int pid);

u16  SPEED_GetCanSpeed(void);

void SPEED_SetCanBusStatus(u8 mode);
u8   SPEED_GetCanBusStatus(void);

#endif
