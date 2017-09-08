#ifndef __LED_H
#define __LED_H	 
#include "sys.h"

#define LED_ON() 	PCout(13)=0	
#define LED_OFF() PCout(13)=1	
#define LED()			PCout(13)	

#define LED0 			PAout(8)	// PA8
#define LED1 			PDout(2)	// PD2	

void LED_Init(void);//≥ı ºªØ

		 				    
#endif
