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
	/* ���ù��ܺ�GPIOB�˿�ʱ��ʹ��*/	 
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
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN; // �����������
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
		GPIO_Init(GPIOA, &GPIO_InitStructure);
	}
	for(i = 0; i < 16; i++) {
		if((i==14)||(i==15)||(i==12))
		continue;
		GPIO_InitStructure.GPIO_Pin = i;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN; // �����������
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
		GPIO_Init(GPIOB, &GPIO_InitStructure);;
	}
	for(i = 0; i < 16; i++) {
		GPIO_InitStructure.GPIO_Pin = i;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN; // �����������
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
		GPIO_Init(GPIOC, &GPIO_InitStructure);;
	}
}
void setGPIOModeToAllPins(void) {
	char i=0;
	GPIO_InitTypeDef GPIO_InitStructure; 
	for(i = 0; i < 16; i++) {
		GPIO_InitStructure.GPIO_Pin = i;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN; // �����������
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

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA| RCC_APB2Periph_AFIO|RCC_APB2Periph_GPIOB, ENABLE);//ʹ��GPIOA�͸��ù���ʱ��
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);//�ر�jtag��ʹ��SWD��������SWDģʽ����


	//EXTI_ClearITPendingBit(EXTI_Line11);
	EXTI_ClearITPendingBit(EXTI_Line14);



	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_11;//PC5
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //���ó���������
	GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOC5

	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_14;//PC5
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //���ó���������
	GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��GPIOC5



	//ʹ���ⲿ�жϷ�ʽ

	//GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource11);	//�ж���2����GPIOE.2
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource14);	//�ж���2����GPIOE.2


	EXTI_InitStructure.EXTI_Line = EXTI_Line14;	//���ð������е��ⲿ��·
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;			//�����ⲿ�ж�ģʽ:EXTI��·Ϊ�ж�����
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;  //�����ش���
	EXTI_InitStructure.EXTI_LineCmd =ENABLE;// DISABLE;
	EXTI_Init(&EXTI_InitStructure);	// ��ʼ���ⲿ�ж�

	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn; //ʹ�ܰ������ڵ��ⲿ�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority =EXTI_IRQPriority; 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =EXTI_IRQSubPriority1;  
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //ʹ���ⲿ�ж�ͨ��
	NVIC_Init(&NVIC_InitStructure); //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���
	/*----------------------------------------*/
 
}

void exti_release(void)
{
	NVIC_InitTypeDef  NVIC_InitStructure;
	EXTI_InitTypeDef  EXTI_InitStructure;

	EXTI_InitStructure.EXTI_Line = EXTI_Line14;	//���ð������е��ⲿ��·
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;			//�����ⲿ�ж�ģʽ:EXTI��·Ϊ�ж�����
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;  //�����ش��� 
	EXTI_InitStructure.EXTI_LineCmd =DISABLE;// DISABLE;
	EXTI_Init(&EXTI_InitStructure);	// ��ʼ���ⲿ�ж�

	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn; //ʹ�ܰ������ڵ��ⲿ�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority =EXTI_IRQPriority; 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =EXTI_IRQSubPriority1;  
	NVIC_InitStructure.NVIC_IRQChannelCmd = DISABLE; //ʹ���ⲿ�ж�ͨ��
	NVIC_Init(&NVIC_InitStructure); //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���
		
}
void FRAMEWORK_Init(void)
{
	SystemInit();
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);// �����ж����ȼ�����2 
	delay_init();	    	 	//��ʱ������ʼ��	
	setGPIOModeToAllPins();
	sys_switch();
	LED_Init();		  			//��ʼ����LED���ӵ�Ӳ���ӿ� 
	UART_Init(115200,UART_IRQPriority,UART_IRQSubPriority);
	CAN_GPIO_Config();//CAN�ܽų�ʼ�� 
	CAN_NVIC_Configuration(CAN_IRQPriority ,CAN_IRQSubPriority,ENABLE);
	USART2_Init(115200,UART2_IRQPriority,UART2_IRQSubPriority);
	TIM4_Init(99,7199,TIM4_IRQPriority,TIM4_IRQSubPriority);		//10ms�ж�
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
