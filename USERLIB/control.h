#ifndef _CONTROL_H_
#define _CONTROL_H_

#include <stdint.h>
#include "can.h"

#define  CMDSETMSG    0x09
#define  CMDIDMSG     0x0E
#define  CMDSIZE      CMDIDMSG




#define  REQUEST     0x01
#define  RESPONSE    0x02


#define  CMDSET      0x81
#define  CMDID       0x82


void controlInit(void);
void control(void);


#define  FILTER_ADD   1
#define  FILTER_FLUSH 2
#define  FILTER_RESET 3



typedef struct 
{
	uint8_t  cmd;
	uint8_t  baudrateStatus;
	BAUDRATE baudrate;
	uint8_t  sendstatus;  // 0 : keep; 1 :suspend  2:refresh
	uint8_t  readstatus;
	uint8_t  filtercmd;
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


void  responsefinish(void);
uint8_t isresponse(void);
#endif




