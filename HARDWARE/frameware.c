#include "includes.h"
#include "can.h"
#include "led.h"
#include "usart.h"
#include "usart2.h"
#include "delay.h"
#include "sys.h"
#include "frameware.h"
#include "keyAcc.h"


ErrorStatus HSEStartUpStatus;

static void exti_release(void);
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
void setGPIOModeToAin(void) {  //input mode 
	char i=0;
	GPIO_InitTypeDef GPIO_InitStructure; 
	for(i = 0; i < 16; i++) {
		if((i==3)||(i==11)||(i==8)|(i==15))
		continue;
		GPIO_InitStructure.GPIO_Pin = i;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN; // 复用推挽输出
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
		GPIO_Init(GPIOA, &GPIO_InitStructure);
	}
	for(i = 0; i < 16; i++) {
		if((i==14)||(i==15)||(i==12))
		continue;
		GPIO_InitStructure.GPIO_Pin = i;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN; // 复用推挽输出
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
		GPIO_Init(GPIOB, &GPIO_InitStructure);;
	}
	for(i = 0; i < 16; i++) {
		GPIO_InitStructure.GPIO_Pin = i;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN; // 复用推挽输出
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
		GPIO_Init(GPIOC, &GPIO_InitStructure);;
	}
}
void setGPIOModeToAllPins(void) {
	char i=0;
	GPIO_InitTypeDef GPIO_InitStructure; 
	for(i = 0; i < 16; i++) {
		GPIO_InitStructure.GPIO_Pin = i;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN; // 复用推挽输出
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
		GPIO_Init(GPIOA, &GPIO_InitStructure);
		GPIO_Init(GPIOB, &GPIO_InitStructure);
		GPIO_Init(GPIOC, &GPIO_InitStructure);
		GPIO_Init(GPIOD, &GPIO_InitStructure);
		GPIO_Init(GPIOE, &GPIO_InitStructure);
		GPIO_Init(GPIOF, &GPIO_InitStructure);
		GPIO_Init(GPIOG, &GPIO_InitStructure);
	}
}

void SYSCLKConfig_STOP(void)
{
	/* Enable HSE */
	RCC_HSEConfig(RCC_HSE_ON);

	/* Wait till HSE is ready */
	HSEStartUpStatus = RCC_WaitForHSEStartUp();

	if(HSEStartUpStatus == SUCCESS)
	{
	/* Enable PLL */ 
		RCC_PLLCmd(ENABLE);

		/* Wait till PLL is ready */
		while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)
		{
		}

		/* Select PLL as system clock source */
		RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);

		/* Wait till PLL is used as system clock source */
		while(RCC_GetSYSCLKSource() != 0x08)
		{
		}
	}
}
void EXTI15_10_IRQHandler(void)
{ 	
	if(EXTI_GetITStatus(EXTI_Line11)){	
		exti_release();	
		EXTI_ClearITPendingBit(EXTI_Line11);

		printf("EXTI_Line11 \r\n");
	}else if(EXTI_GetITStatus(EXTI_Line14)){
		exti_release();	
		EXTI_ClearITPendingBit(EXTI_Line14);	  
		printf("EXTI_Line14 \r\n");
	}			
	SCB->SCR |=SCB_SCR_SEVONPEND;
	printf("%s \r\n",__func__);
}

void exti_Init(void)
{	
	GPIO_InitTypeDef  GPIO_InitStructure;  		  
	NVIC_InitTypeDef  NVIC_InitStructure;
	EXTI_InitTypeDef  EXTI_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA| RCC_APB2Periph_AFIO|RCC_APB2Periph_GPIOB, ENABLE);//使能GPIOA和复用功能时钟
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);//关闭jtag，使能SWD，可以用SWD模式调试


	//EXTI_ClearITPendingBit(EXTI_Line11);
	EXTI_ClearITPendingBit(EXTI_Line14);



	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_11;//PC5
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //设置成上拉输入
	GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOC5

	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_14;//PC5
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //设置成上拉输入
	GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化GPIOC5



	//使用外部中断方式

	//GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource11);	//中断线2连接GPIOE.2
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource14);	//中断线2连接GPIOE.2


	EXTI_InitStructure.EXTI_Line = EXTI_Line14;	//设置按键所有的外部线路
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;			//设外外部中断模式:EXTI线路为中断请求
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;  //上升沿触发
	EXTI_InitStructure.EXTI_LineCmd =ENABLE;// DISABLE;
	EXTI_Init(&EXTI_InitStructure);	// 初始化外部中断

	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn; //使能按键所在的外部中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority =EXTI_IRQPriority; 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =EXTI_IRQSubPriority1;  
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //使能外部中断通道
	NVIC_Init(&NVIC_InitStructure); //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器
	/*----------------------------------------*/
 
}

void exti_release(void)
{
	NVIC_InitTypeDef  NVIC_InitStructure;
	EXTI_InitTypeDef  EXTI_InitStructure;

	EXTI_InitStructure.EXTI_Line = EXTI_Line14;	//设置按键所有的外部线路
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;			//设外外部中断模式:EXTI线路为中断请求
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;  //上升沿触发 
	EXTI_InitStructure.EXTI_LineCmd =DISABLE;// DISABLE;
	EXTI_Init(&EXTI_InitStructure);	// 初始化外部中断

	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn; //使能按键所在的外部中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority =EXTI_IRQPriority; 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =EXTI_IRQSubPriority1;  
	NVIC_InitStructure.NVIC_IRQChannelCmd = DISABLE; //使能外部中断通道
	NVIC_Init(&NVIC_InitStructure); //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器
		
}
void FRAMEWORK_Init(void)
{
	SystemInit();
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);// 设置中断优先级分组2 
	delay_init();	    	 	//延时函数初始化	
	setGPIOModeToAllPins();
	sys_switch();
	LED_Init();		  			//初始化与LED连接的硬件接口 
	UART_Init(115200,UART_IRQPriority,UART_IRQSubPriority);
	CAN_GPIO_Config();//CAN管脚初始化 
	CAN_NVIC_Configuration(CAN_IRQPriority ,CAN_IRQSubPriority,ENABLE);
	USART2_Init(115200,UART2_IRQPriority,UART2_IRQSubPriority);
	TIM4_Init(99,7199,TIM4_IRQPriority,TIM4_IRQSubPriority);		//10ms中断
	TIM4_Set(0);	
	KEYACC_Init();	
}
void FRAMEWORK_Release(void)
{
	CAN_NVIC_Configuration(CAN_IRQPriority ,CAN_IRQSubPriority,DISABLE);
	TIM_ITConfig(TIM4,TIM_IT_Update,DISABLE); 
	TIM4_Set(0);	
	setGPIOModeToAin();
}
