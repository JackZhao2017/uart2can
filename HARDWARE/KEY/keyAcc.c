#include "keyAcc.h"
#include "delay.h"

 	    
//������ʼ������ 
//PA15��PC5 ���ó�����
void KEYACC_Init(void)
{
	
	GPIO_InitTypeDef GPIO_InitStructure;

 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);//ʹ��PORTA,PORTCʱ��

		
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_14;//PC5
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //���ó���������
 	GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��GPIOC5
	
} 


u8 KEYACC_Scan(void)
{	  	     
	return KeyAcc;// �ް�������
}
