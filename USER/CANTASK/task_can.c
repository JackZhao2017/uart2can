/*
	Created by Jack 2017/9/7
*/

#include "includes.h"
#include "can.h"
#include "task_config.h"
#include "stdio.h"

#include "common_convert.h"

#include "task_can_send.h"
#include "task_can_read.h"
#include "can_queue_read.h"
#include "can_queue_send.h"


FILTER default_filter[]={
						{0x0cfe6c00,0x00FFFF00,EXT},//7、8字节为转速计输出车速。
						{0x0cfef100,0x00FFFF00,EXT},//2、3字节为基于车轮的车速。
						{0x00f00000,0x00FFFF00,EXT},//减速器控制器;
						{0x00f00300,0x00FFFF00,EXT},//2字节 AP加速踏板位置。
						{0x00f00400,0x00FFFF00,EXT},//发动机、减速器模式
						{0x00f00100,0x00FFFF00,EXT},//刹车踏板位置
						{0x00fE6f00,0x00FFFF00,EXT},//可适应巡航速度
						{0x00fEC200,0x00FFFF00,EXT},//电子控制器输出状态
						{0x00fEFA00,0x00FFFF00,EXT},//电子控制器输出状态
					};

static	uint8_t g_CAN_Prescaler=0;
extern	QUEUE_READ_INFO CAN_QUEUE_INFO;

uint8_t CAN_GetReadMsg(CanRxMsg *msg)
{
	if(CAN_QueueReadGet(&CAN_QUEUE_INFO,msg)){

		return 1;
	}
	return 0;	
}

uint8_t CAN_GetHeadReadMsg(CanRxMsg **msg)
{
	if(CAN_QueueReadGetHead(&CAN_QUEUE_INFO,msg)){
		return 1;
	}
	return 0;	
}
void   CAN_DelHeadReadMsg(void)
{
	CAN_QueueReadDelHead(&CAN_QUEUE_INFO);
}

void CAN_TaskInit(BAUDRATE brandrate)
{

	g_CAN_Prescaler=brandrate.CAN_Prescaler;

	CAN_INIT(brandrate);
	CAN_FILTERINIT(default_filter,9);	

	//CAN_SendTaskInit();
	CAN_ReadTaskInit();

	printf("can_task_init\r\n");
}

void CAN_SendPutMsg(u8 *msg)
{
	if(!CAN_GetSendTaskStatus()){
		printf("can send task suspend!\r\n");
		return ;
	}
	CAN_SendOneFrame(msg);
}

uint8_t  CAN_TaskPause(void)
{
	if(CAN_SendTaskPause()==0){
		goto err;
	}

	if(CAN_ReadTaskPause()==0){
		goto err;
	} 

	return 1;
err:
	CAN_SendTaskStart();
	CAN_ReadTaskStart();
	return 0;
}


uint8_t CAN_TaskStart(void)
{
	uint8_t val=0;

	if(CAN_SendTaskStart()){
		val|=1;
	}

	if(CAN_ReadTaskStart())
	{
		val|=2;
	}

	return val;
}

void  CAN_SetBrandrate(BAUDRATE brandrate)
{
	CAN_INIT(brandrate);
}
uint8_t CAN_GetBrandrate(void)
{
	return g_CAN_Prescaler;
}
void	CAN_SetFilter(FILTER *filter,uint8_t len)
{
	CAN_FILTERINIT(filter,len);	
}

