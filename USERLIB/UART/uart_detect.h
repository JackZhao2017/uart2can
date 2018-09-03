#ifndef _UARTDETECT_H_
#define _UARTDETECT_H_
#include <stdint.h>

#define  CANMSG      0x11 
#define  MSY_SYNC    0x55

unsigned char   UART_DetectvaildSync(unsigned char *buf,unsigned char index);
unsigned char   UART_DetectvaildMsgtype(unsigned char *buf,unsigned char *type);
unsigned char   UART_Detectcrc8(unsigned char *msg,unsigned char  start, unsigned char  len);

#endif
