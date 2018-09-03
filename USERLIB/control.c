#include "cmd_queue.h"
#include "control.h"
#include "includes.h"
#include "task_config.h"
#include "task_can.h"
#include "uart_encode.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

static 	 OS_EVENT * g_mutex_control;
#define  lock     OSMutexPend(g_mutex_control,0,&err)
#define  unlock   OSMutexPost(g_mutex_control)
static  uint8_t g_isresponse=0;
CMD_QUEUE_INFO CMDINFO;
SETCMD   			 setcmd;
IDCMD    			 idcmd[FILTER_NUM];
static  uint8_t parsecmd[CMDSIZE];
FILTER mfilter[FILTER_NUM]={{0x0cfe6c00,0x00FFFF00,EXT},{0x0cfef100,0x00FFFF00,EXT}};
extern FILTER default_filter[2];
void controlInit(void)
{
	uint8_t err;
	memset(&setcmd,0,sizeof(SETCMD));
	memset(idcmd,0,sizeof(IDCMD));
	setcmd.baudrate.CAN_SJW=CAN_SJW_1tq;
	setcmd.baudrate.CAN_BS1=CAN_BS1_3tq;
	setcmd.baudrate.CAN_BS2=CAN_BS2_2tq;
	setcmd.baudrate.CAN_Prescaler=CAN_250kbps;	
 	g_mutex_control=OSMutexCreate(CMD_MUTEX_PRIO,&err);///???????
	cmd_queue_init(&CMDINFO,CMDQUEUESIZE);
}

void putcontrolCmd(uint8_t *data)
{
	uint8_t err;
	lock;
  put_cmd_queue(&CMDINFO,data);
	unlock;
}
char getcontrolCmd(uint8_t *data)
{
	uint8_t err;
	lock;
	if(get_cmd_queue(&CMDINFO,data)){
		unlock;
		return 1;
	}
	unlock;
	return 0;	
}

char cmdsetparse(uint8_t *parsecmd)
{
	  //baudrate
		if(parsecmd[3]==1){                
			  setcmd.baudrateStatus=1;
			  setcmd.baudrate.CAN_Prescaler=parsecmd[4];
		}else if(parsecmd[3]==2){
			  setcmd.baudrateStatus=2;
				setcmd.baudrate.CAN_Prescaler=CAN_250kbps;
		}else{
				setcmd.baudrateStatus=0;
		}
	  // can send task status cmd
	  if(parsecmd[5]<3){
				setcmd.sendstatus=parsecmd[5];
		}
		// can send task status cmd
		if(parsecmd[6]<3){
			  setcmd.readstatus=parsecmd[6];
		}
		// can filter status 
		if(parsecmd[7]<4){
				setcmd.filtercmd=parsecmd[7];
		}

		printf("CAN_SJW : 0x%02x\r\n",setcmd.baudrate.CAN_SJW);
		printf("CAN_BS1 : 0x%02x\r\n",setcmd.baudrate.CAN_BS1);
		printf("CAN_BS2 : 0x%02x\r\n",setcmd.baudrate.CAN_BS2);
		printf("CAN_Pre : 0x%02x\r\n",setcmd.baudrate.CAN_Prescaler);
		printf("status_t: 0x%02x\r\n",setcmd.sendstatus);
		printf("status_r: 0x%02x\r\n",setcmd.readstatus);
		printf("filter  : 0x%02x\r\n",setcmd.filtercmd);
		return 1;
}

char canidparse(uint8_t *parsecmd)
{
	  uint8_t index=parsecmd[3];
		if(index<6){
				idcmd[index].cred=1;
			  idcmd[index].fmt=parsecmd[4];
			  memcpy(&idcmd[index].id,&parsecmd[5],4);
			  memcpy(&idcmd[index].mask,&parsecmd[9],4);
		}
		return 1;
}
uint8_t  cmdid2filter(IDCMD *idcmd,FILTER *filter)
{
	    uint8_t i=0;
	    uint8_t count=0;
	    for(i=0;i<FILTER_NUM;i++){
		     if(idcmd[i].cred!=1){
							continue;
				 }
				 filter[count].CAN_ID=idcmd[i].id;
				 filter[count].CAN_ID_FMT=idcmd[i].fmt;
				 filter[count].CAN_ID_MASK=idcmd[i].mask;
				 ++count;
		 }	 
		 return count;
}
uint8_t  addcmdid2filter(IDCMD *idcmd,FILTER *filter)
{
	    uint8_t i=0;
	    uint8_t count=2;
	    for(i=0;i<FILTER_NUM-2;i++){
		     if(idcmd[i].cred!=1){
							continue;
				 }
				 filter[count].CAN_ID=idcmd[i].id;
				 filter[count].CAN_ID_FMT=idcmd[i].fmt;
				 filter[count].CAN_ID_MASK=idcmd[i].mask;
				 ++count;
		 }	 
		 return count;
}
char  cmdsetprocess(SETCMD *setcmd)
{
	   uint8_t count=0;
	   if(setcmd->baudrateStatus||setcmd->filtercmd){
				if(!CAN_GetSendTaskStatus()&&!CAN_GetReadTaskStatus()){
					 if(setcmd->baudrateStatus){
							setcmd->baudrate.CAN_SJW=CAN_SJW_1tq;
							setcmd->baudrate.CAN_BS1=CAN_BS1_3tq;
							setcmd->baudrate.CAN_BS2=CAN_BS2_2tq;
							CAN_SetBrandrate(setcmd->baudrate);
					 }
					 if(setcmd->filtercmd==FILTER_ADD){
							count=addcmdid2filter(idcmd,mfilter);
							if(count>0){
									CAN_SetFilter(mfilter,count);
							}				
					 }else if(setcmd->filtercmd==FILTER_FLUSH){
							count=cmdid2filter(idcmd,mfilter);
							if(count>0){
									CAN_SetFilter(mfilter,count);
							}
					 }else if(setcmd->filtercmd==FILTER_RESET){
							CAN_SetFilter(default_filter,2);
					 }
				}
		 }
		 
		 if((setcmd->sendstatus==1)||(setcmd->readstatus==1)){
				if(setcmd->sendstatus==1){
						CAN_SendTaskPause();			
				}
				if(setcmd->readstatus==1){
						CAN_ReadTaskPause();			
				}				
		 }else if((setcmd->sendstatus==2)||(setcmd->readstatus==2)){
				
			  if(setcmd->sendstatus==2){
						CAN_SendTaskStart();			
				}
				if(setcmd->readstatus==2){
						CAN_ReadTaskStart();			
				}			
		 }else{
			  CAN_TaskStart();
		 }
	   return 1;
}
uint8_t isresponse(void)
{
		return g_isresponse;
}
void  responsefinish(void)
{	
		g_isresponse=0;
}
void control(void)
{
		if(getcontrolCmd(parsecmd)){	
				if(parsecmd[2]==CMDSET){
						cmdsetparse(parsecmd);
						cmdsetprocess(&setcmd);
				}else if(parsecmd[2]==CMDID){
						canidparse(parsecmd);				 
				}
				UART_ENCODE_AddSystermErrorcode(parsecmd[1]|0x80);
				g_isresponse=1;
		}
}

