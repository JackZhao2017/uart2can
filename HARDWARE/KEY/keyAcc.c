#include "keyAcc.h"
#include "delay.h"

 	    
//按键初始化函数 
//PA15和PC5 设置成输入
void KEYACC_Init(void)
{
	
	GPIO_InitTypeDef GPIO_InitStructure;

 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);//使能PORTA,PORTC时钟

		
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_14;//PC5
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //设置成上拉输入
 	GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化GPIOC5
	
} 


u8 KEYACC_Scan(void)
{	  	     
	return KeyAcc;// 无按键按下
}
