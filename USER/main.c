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
#include "uart_task.h"
#include "crc8.h"



OS_STK START_TASK_STK[START_STK_SIZE];//任务堆栈	
void start_task(void *pdata);					//任务函数
BAUDRATE baudrate;

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
		uart_init(921600);	
	  CAN_GPIO_Config();					//CAN管脚初始化 
		USART2_Init(115200);
		OSInit();   
		OSTaskCreate(start_task,(void *)0,(OS_STK *)&START_TASK_STK[START_STK_SIZE-1],START_TASK_PRIO );//创建起始任务
		OSStart();	
}

void start_task(void *pdata)
{
		OS_CPU_SR cpu_sr=0;
		pdata = pdata; 
	  OSTimeDlyHMSM(0,0,2,100);
		OS_ENTER_CRITICAL();						//进入临界区(无法被中断打断)   
		printf("start_task\r\n");
		crcInit(LSB,POLY);
	  baudrate.CAN_SJW=CAN_SJW_1tq;
	  baudrate.CAN_BS1=CAN_BS1_3tq;
	 	baudrate.CAN_BS2=CAN_BS2_2tq;
	  baudrate.CAN_Prescaler=CAN_250kbps;
		can_task_init(baudrate);
	 	uart_task_Init();
    schedule_task_init();	
		OSTaskSuspend(START_TASK_PRIO);	//挂起起始任务.
		OS_EXIT_CRITICAL();							//退出临界区(可以被中断打断)
}






