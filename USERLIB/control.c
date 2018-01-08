#include "cmd_queue.h"
#include "control.h"
#include "includes.h"
#include "taskconfig.h"
#include "can_task.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

static 	 OS_EVENT * g_mutex_control;
#define  lock     OSMutexPend(g_mutex_control,0,&err)
#define  unlock   OSMutexPost(g_mutex_control)

CMD_QUEUE_INFO CMDINFO;
SETCMD   			 setcmd;
IDCMD    			 idcmd[FILTER_NUM];
static  uint8_t parsecmd[CMDSIZE];
extern FILTER filter[FILTER_NUM];
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
		if(parsecmd[2]==1){
			setcmd.status=parsecmd[2];
		
			if(parsecmd[3]<4){
				setcmd.baudrate.CAN_SJW=parsecmd[3];
			}
			if(parsecmd[4]<0x10){
				setcmd.baudrate.CAN_BS1=parsecmd[4];
			}
			if(parsecmd[5]<0x08){
				setcmd.baudrate.CAN_BS2=parsecmd[5];
			}
			if(parsecmd[6]>0){
			  setcmd.baudrate.CAN_Prescaler=parsecmd[6];
			}
			if(parsecmd[7]<FILTER_NUM+1){
			   setcmd.idsum=parsecmd[7];
			}else{
			   setcmd.idsum=FILTER_NUM;
			}
	    }else if(parsecmd[2]==2){
		    setcmd.status=parsecmd[2];
		    setcmd.isSend=parsecmd[8];
		}
		printf("CAN_SJW : 0x%02x\r\n",setcmd.baudrate.CAN_SJW);
		printf("CAN_BS1 : 0x%02x\r\n",setcmd.baudrate.CAN_BS1);
		printf("CAN_BS2 : 0x%02x\r\n",setcmd.baudrate.CAN_BS2);
		printf("CAN_Pre : 0x%02x\r\n",setcmd.baudrate.CAN_Prescaler);
		printf("status  : 0x%02x\r\n",setcmd.status);
		printf("idsum   : 0x%02x\r\n",setcmd.idsum);
		return 1;
}

char canidparse(uint8_t *parsecmd)
{
		if(parsecmd[2]<setcmd.idsum){
				idcmd[parsecmd[2]].index=parsecmd[2];
			    idcmd[parsecmd[2]].cred=1;
				memcpy(&idcmd[parsecmd[2]].id, &parsecmd[3],4);
				memcpy(&idcmd[parsecmd[2]].mask, &parsecmd[7],4);
				idcmd[parsecmd[2]].fmt=parsecmd[11];
				printf("index : 0x%02x\r\n",idcmd[parsecmd[2]].index);
				printf("cred  : 0x%02x\r\n",idcmd[parsecmd[2]].cred);
				printf("id    : 0x%x\r\n",idcmd[parsecmd[2]].id);
				printf("mask  : 0x%x\r\n",idcmd[parsecmd[2]].mask);
			    printf("fmt   : 0x%x\r\n",idcmd[parsecmd[2]].fmt);
		}else{
				return 0;
		}
		return 1;
}
uint8_t  cmdid2filter(IDCMD *idcmd,FILTER *f)
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

char  cmdsetprocess(SETCMD *setcmd)
{
	   uint8_t count=0;
	   if(setcmd->status==1){
	   		pause_can_task();
	   }else if(setcmd->status==2){		    
	   	    can_set_brandrate(setcmd->baudrate);
			count = cmdid2filter(idcmd,filter);
			can_set_filter(filter,count);
	   		start_canread_task();
	   		if(setcmd->isSend){
						start_cansend_task();
	   		}
	   }
	   return 1;
}

void control(void)
{
		if(getcontrolCmd(parsecmd)){	
				if(parsecmd[1]==CMDSET){
						cmdsetparse(parsecmd);
						cmdsetprocess(&setcmd);
				}else if(parsecmd[1]==CMDID){
						canidparse(parsecmd);
					 
				}
		}
}

