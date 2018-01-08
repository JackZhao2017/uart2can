#ifndef _CONVERT_H_
#define _CONVERT_H_

#include "Encode.h"
#include "can.h"
void  canconvertuart(CanRxMsg *canmsg,UartTxMsg *uartmsg);
void  uartconvertcan(UartRxMsg *uartmsg,CanTxMsg *canmsg);
#endif


