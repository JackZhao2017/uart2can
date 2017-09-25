#ifndef __CAN_H
#define	__CAN_H


#include "stm32f10x.h" 

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

typedef struct{
		volatile u8 cur_mode;
		volatile u8 test_mode;
		volatile u8 test_timeout;
} CANBUS_MODE;

typedef struct{
		u32 CAN_ID;
		u32 CAN_ID_MASK;
		u32 CAN_ID_FMT;
} FILTER;


 void CAN_INIT(int baudrate);
 void CAN_FILTERINIT(FILTER *filter,int len);
 void can_tx(u32 id,u32 id_fmt,u8 *data,u8 len);
 void can_rx(void);
 void CAN_NVIC_Configuration(void);
 void CAN_GPIO_Config(void);

#endif /* __CAN_H */


