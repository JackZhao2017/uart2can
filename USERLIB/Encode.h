
#ifndef _ENCODE_H_
#define _ENCODE_H_

#include <stdint.h>

typedef struct {
	  uint8_t  SYNC;
	  uint8_t  TYPE;
	  uint8_t  IDE;
	  uint8_t  RTR;
	  uint32_t ID;
	  uint8_t  DLC;  
	  uint8_t  Data[8];
	  uint8_t  CRC8;
}UartTxMsg;

typedef struct {
	  uint8_t  TYPE;
	  uint8_t  IDE;
	  uint8_t  DLC;
	  uint8_t  RTR;
	  uint32_t ID;  
	  uint8_t  Data[8];
}UartRxMsg;

#define  CANMSG      0x10 

uint8_t get_send_packet( UartTxMsg *usrtmsg);

#endif 

