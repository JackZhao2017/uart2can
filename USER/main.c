/*
	Created by Jack 2017/9/7
*/
#include "led.h"
#include "delay.h"
#include "sys.h"
#include "includes.h" 
#include "taskconfig.h"
#include "can.h"
#include "usart.h"
#include "usart2.h"
#include "schedule_task.h"
#include "can_task.h"
#include "crc8.h"

FILTER filter[1]={{0x0cfe6c00,0x00ffff00,EXT}};

OS_STK START_TASK_STK[START_STK_SIZE];//任务堆栈	
void start_task(void *pdata);					//任务函数

void sys_switch(void)
{
	GPIO_InitTypeDef GPIO_InitStructure; 
  /* 复用功能和GPIOB端口时钟使能*/	 
		RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB, ENABLE);	                        											 
  /* Configure CAN pin: RX */	 
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 ;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	PBout(12)=0;
}

int main()
{
		delay_init();	    	 	//延时函数初始化	
		sys_switch();
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);// 设置中断优先级分组2
		LED_Init();		  			//初始化与LED连接的硬件接口
		uart_init(115200);
		crcInit(LSB,POLY);
		CAN_GPIO_Config();					//CAN管脚初始化
	  CAN_NVIC_Configuration(); 	//CAN中断初始化   
		CAN_INIT(CAN_250kbps);
		CAN_FILTERINIT(filter,sizeof(filter)/sizeof(FILTER));	
		USART2_Init(115200);
		OSInit();   
		OSTaskCreate(start_task,(void *)0,(OS_STK *)&START_TASK_STK[START_STK_SIZE-1],START_TASK_PRIO );//创建起始任务
		OSStart();	
}

void start_task(void *pdata)
{
		OS_CPU_SR cpu_sr=0;
		pdata = pdata; 
		delay_ms(1000);
		delay_ms(1000);
		OS_ENTER_CRITICAL();						//进入临界区(无法被中断打断)   
		printf("start_task\r\n");
		can_task_init();
		OSTaskCreate(schedule_indicate_task,(void *)0,(OS_STK*)&SCHEDULE_INDICATE_TASK_STK[SCHEDULE_INDICATE_STK_SIZE-1],SCHEDULE_INDICATE_TASK_PRIO);
	  
		OSTaskSuspend(START_TASK_PRIO);	//挂起起始任务.
		OS_EXIT_CRITICAL();							//退出临界区(可以被中断打断)
}






