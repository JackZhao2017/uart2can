#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sleepwakeup.h"
#include "includes.h"
#include "frameware.h"
#include "led.h"
#include "task_uart.h"
#include "task_can.h"


void sys_sleep_mode(void)
{
	SysTick->CTRL&=~SysTick_CTRL_TICKINT_Msk;	
	NVIC_SystemLPConfig(NVIC_LP_SLEEPDEEP,DISABLE);
	NVIC_SystemLPConfig(NVIC_LP_SLEEPONEXIT,DISABLE);
	__WFI();                                                 /* Request Wait For Interrupt */
}
void sys_stop_mode(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR,ENABLE);  //使能时钟
	PWR_EnterSTOPMode(PWR_Regulator_LowPower,PWR_STOPEntry_WFI);	 
}


static void switchoffclk(void)
{ 
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_CRC|RCC_AHBPeriph_SDIO,DISABLE);

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOD|RCC_APB2Periph_GPIOE|RCC_APB2Periph_GPIOF|RCC_APB2Periph_GPIOG,DISABLE);

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1|RCC_APB2Periph_ADC2|RCC_APB2Periph_SPI1|RCC_APB2Periph_TIM8|RCC_APB2Periph_ADC3,DISABLE);

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1|RCC_APB2Periph_TIM15|RCC_APB2Periph_TIM16|RCC_APB2Periph_TIM17|RCC_APB2Periph_TIM9|RCC_APB2Periph_TIM10|RCC_APB2Periph_TIM11,DISABLE);

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2|RCC_APB1Periph_TIM3|RCC_APB1Periph_TIM4|RCC_APB1Periph_TIM5|RCC_APB1Periph_TIM6|\
	                       RCC_APB1Periph_TIM7|RCC_APB1Periph_TIM12|RCC_APB1Periph_TIM13|RCC_APB1Periph_TIM14,DISABLE);

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_WWDG|RCC_APB1Periph_SPI2|RCC_APB1Periph_SPI3|RCC_APB1Periph_USART3|RCC_APB1Periph_UART4|RCC_APB1Periph_UART5|RCC_APB1Periph_I2C1|RCC_APB1Periph_I2C2\
	                     |RCC_APB1Periph_USB|RCC_APB1Periph_BKP|RCC_APB1Periph_DAC|RCC_APB1Periph_CEC,DISABLE);
}
void sys_mode_entry(void)
{
	OS_CPU_SR cpu_sr=0;
	CAN_TaskPause();
	UART_TaskPause();
	switchoffclk();

	OS_ENTER_CRITICAL();
	exti_Init();
	OS_EXIT_CRITICAL();	
}
void entry_sleep_mode(void)
{   
	printf("%s start\r\n",__func__);
	sys_mode_entry();
	sys_stop_mode();
	FRAMEWORK_Init();
	UART_TaskStart();
	CAN_TaskStart();
	printf("%s exit\r\n",__func__);
}
 




