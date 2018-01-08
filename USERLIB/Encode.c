
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Encode.h"
#include "can_queue.h"
#include "can_task.h"
#include "convert.h"

uint8_t get_send_packet( UartTxMsg *usrtmsg)
{
		CanRxMsg canmsg; 
	    if(getCanMsg(&canmsg)){
			 canconvertuart(&canmsg,usrtmsg);
			 return 1;
		}
		return 0;
}






