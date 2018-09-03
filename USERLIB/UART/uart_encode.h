
#ifndef _UARTENCODE_H_
#define _UARTENCODE_H_

#include <stdint.h>

typedef struct {
	uint8_t  SYNC;
	uint8_t  TYPE;
	uint8_t  COUNTER;
	uint8_t  IDE_RTR;
	uint32_t ID;
	uint8_t  DLC;  
	uint8_t  Data[8];
	uint8_t  CRC8;
}UartTxMsg;

typedef struct {
	uint8_t  SYNC;
	uint8_t  TYPE;
	uint8_t  COUNTER;
	uint8_t  TASK_S;
	uint8_t  DATA_S;		
	uint8_t  BITRATE;
	uint8_t  ERRCODE;
	uint8_t  CRC8;
}SysMsg;

typedef struct {
	uint8_t  TYPE;
	uint8_t  COUNTER;
	uint8_t  IDE_RTR;
	uint8_t  DLC;
	uint32_t ID;  
	uint8_t  Data[8];
}UartRxMsg;

#define  SYSMSG      0x07


uint8_t UART_ENCODE_GetSend(UartTxMsg *uartmsg);
void    UART_ENCODE_GetSystermStatus(SysMsg *sysmsg);
void    UART_ENCODE_AddSystermErrorcode(uint8_t err);

#endif 

