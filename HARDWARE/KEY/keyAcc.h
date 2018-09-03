#ifndef __KEYACC_H
#define __KEYACC_H	 
 


#define KEY1  GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_15)//读取按键1
#define KeyAcc  GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_14)//读取ACC
 



void KEYACC_Init(void);//IO初始化
unsigned char   KEYACC_Scan(void);  	//按键扫描函数					    
#endif
