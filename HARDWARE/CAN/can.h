#ifndef __CAN_H
#define	__CAN_H


#include "stm32f10x.h" 
#include <stdint.h>

#define STD 11
#define EXT 29

#define CAN_500kbps  12
#define CAN_250kbps  24
#define CAN_100kpbs  60

#define  OBDII_CMD_ID     0x7df
#define  OBDII_SPEED_PID  0xd


#define  SAE_1939			0x01
#define  OBDII				0x02
#define  ERR					-1


#define  FILTER_NUM   6

typedef struct 
{
	uint8_t  CAN_SJW;
	uint8_t  CAN_BS1;
	uint8_t  CAN_BS2;
	uint8_t  CAN_Prescaler;
}BAUDRATE;


typedef struct{
		u32 CAN_ID;
		u32 CAN_ID_MASK;
		u8  CAN_ID_FMT;
} FILTER;


 void CAN_INIT(BAUDRATE baudrate);
 void CAN_FILTERINIT(FILTER *filter,uint8_t len);
 void can_tx(u32 id,u32 id_fmt,u8 *data,u8 len);
 void CAN_TRANSMIT(CanTxMsg *TxMessage);
 void can_rx(void);
 void CAN_NVIC_Configuration(uint8_t Priority ,uint8_t SubPriority,FunctionalState newstate);
 void CAN_GPIO_Config(void);

#endif /* __CAN_H */


