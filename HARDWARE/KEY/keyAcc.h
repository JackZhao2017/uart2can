#ifndef __KEYACC_H
#define __KEYACC_H	 
 


#define KEY1  GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_15)//��ȡ����1
#define KeyAcc  GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_14)//��ȡACC
 



void KEYACC_Init(void);//IO��ʼ��
unsigned char   KEYACC_Scan(void);  	//����ɨ�躯��					    
#endif
