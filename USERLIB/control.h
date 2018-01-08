#ifndef _CONTROL_H_
#define _CONTROL_H_

#include <stdint.h>
#include "can.h"
#define  CMDMSG      0x0c
#define  CMDSIZE      CMDMSG

#define  REQUEST     0x01
#define  RESPONSE    0x02


#define  CMDSET      0x81
#define  CMDID       0x82


void controlInit(void);
void control(void);


typedef struct 
{
	uint8_t  cmd;
	uint8_t  status;  // 0 : keep; 1 :suspend  2:refresh
	uint8_t  cred;
	BAUDRATE baudrate;
	uint8_t  isSend;
	uint8_t  idsum; 
	
}SETCMD;

typedef struct 
{
	uint8_t  cmd;
	uint8_t  cred;
	uint8_t  index;
	uint8_t  fmt;
	uint32_t id;
	uint32_t mask;
}IDCMD;
void control(void);
void controlInit(void);

char getcontrolCmd(uint8_t *data);
void putcontrolCmd(uint8_t *data);

#endif




