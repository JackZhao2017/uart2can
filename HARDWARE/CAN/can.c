
#include "can.h"
#include "led.h"
#include "stdio.h"
#include "string.h"
#include "can_speed.h"
#include "includes.h"
#include "taskconfig.h"
#include "can_task.h"

typedef enum {FAILED = 0, PASSED = !FAILED} TestStatus;

/* 在中断处理函数中返回 */
__IO uint32_t ret = 0;

volatile TestStatus TestRx;	

/*CAN RX0 中断优先级配置  */
 void CAN_NVIC_Configuration(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;

  	/* Configure the NVIC Preemption Priority Bits */  
  
	#ifdef  VECT_TAB_RAM  
	  /* Set the Vector Table base location at 0x20000000 */ 
	  NVIC_SetVectorTable(NVIC_VectTab_RAM, 0x0); 
	#else  /* VECT_TAB_FLASH  */
	  /* Set the Vector Table base location at 0x08000000 */ 
	  NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);   
	#endif

	/* enabling interrupt */
  	NVIC_InitStructure.NVIC_IRQChannel=USB_LP_CAN1_RX0_IRQn;;
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  	NVIC_Init(&NVIC_InitStructure);
}

/*CAN GPIO 和时钟配置 */
 void CAN_GPIO_Config(void)
{
  GPIO_InitTypeDef GPIO_InitStructure; 
  /* 复用功能和GPIOB端口时钟使能*/	 
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOA, ENABLE);	                        											 

  /* CAN1 模块时钟使能 */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE); 

	//GPIO_PinRemapConfig(GPIO_Remap1_CAN1, ENABLE);
  /* Configure CAN pin: RX */	 
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11  ;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	 // 上拉输入
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  
  /* Configure CAN pin: TX */   
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; // 复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

	//#define GPIO_Remap_CAN    GPIO_Remap1_CAN1 本实验没有用到重映射I/O
  // 
}

/*	CAN初始化 */
 void CAN_INIT(int baudrate)
{
  CAN_InitTypeDef        CAN_InitStructure;

  /* CAN register init */
  CAN_DeInit(CAN1);	//将外设CAN的全部寄存器重设为缺省值
  CAN_StructInit(&CAN_InitStructure);//把CAN_InitStruct中的每一个参数按缺省值填入

  /* CAN cell init */
  CAN_InitStructure.CAN_TTCM=DISABLE;//没有使能时间触发模式
  CAN_InitStructure.CAN_ABOM=DISABLE;//没有使能自动离线管理
  CAN_InitStructure.CAN_AWUM=DISABLE;//没有使能自动唤醒模式
  CAN_InitStructure.CAN_NART=DISABLE;//没有使能非自动重传模式
  CAN_InitStructure.CAN_RFLM=DISABLE;//没有使能接收FIFO锁定模式
  CAN_InitStructure.CAN_TXFP=DISABLE;//没有使能发送FIFO优先级
  CAN_InitStructure.CAN_Mode=CAN_Mode_Normal;//CAN设置为正常模式
  CAN_InitStructure.CAN_SJW=CAN_SJW_1tq; //重新同步跳跃宽度1个时间单位
  CAN_InitStructure.CAN_BS1=CAN_BS1_3tq; //时间段1为3个时间单位
  CAN_InitStructure.CAN_BS2=CAN_BS2_2tq; //时间段2为2个时间单位
  CAN_InitStructure.CAN_Prescaler=baudrate;  //时间单位长度为60	
  CAN_Init(CAN1,&CAN_InitStructure);
                                      //波特率为：72M/2/60(1+3+2)=0.1 即100K
  /* CAN FIFO0 message pending interrupt enable */ 
	CAN_ITConfig(CAN1,CAN_IT_FMP0, ENABLE); //使能FIFO0消息挂号中断
 
 }  

 
void CAN_FILTERINIT(FILTER *filter,int len)
{
		int i=0;
		unsigned int filterId;
		unsigned int filterMaskId;
		CAN_FilterInitTypeDef  CAN_FilterInitStructure; 
	  /* CAN filter init */
		for(i=0;i<len;i++){
			filterId=filter[i].CAN_ID<<(32-filter[i].CAN_ID_FMT);
			filterMaskId=filter[i].CAN_ID_MASK<<(32-filter[i].CAN_ID_FMT);
			CAN_FilterInitStructure.CAN_FilterNumber=i;//指定过滤器为1
			CAN_FilterInitStructure.CAN_FilterMode=CAN_FilterMode_IdMask;//指定过滤器为标识符屏蔽位模式
			CAN_FilterInitStructure.CAN_FilterScale=CAN_FilterScale_32bit;//过滤器位宽为32位
			CAN_FilterInitStructure.CAN_FilterIdHigh=(filterId>>16)&0xffff;// 过滤器标识符的高16位值
			if(filter[i].CAN_ID_FMT==EXT){
					CAN_FilterInitStructure.CAN_FilterIdLow=(filterId&0xffff)|CAN_ID_EXT;//	 过滤器标识符的低16位值
			}else{
					CAN_FilterInitStructure.CAN_FilterIdLow=filterId&0xffff;
			}
			CAN_FilterInitStructure.CAN_FilterMaskIdHigh=(filterMaskId>>16)&0xffff;//过滤器屏蔽标识符的高16位值
			CAN_FilterInitStructure.CAN_FilterMaskIdLow=(filterMaskId&0xffff)|0x6;//	过滤器屏蔽标识符的低16位值
			CAN_FilterInitStructure.CAN_FilterFIFOAssignment=CAN_FIFO0;// 设定了指向过滤器的FIFO为0
			CAN_FilterInitStructure.CAN_FilterActivation=ENABLE;// 使能过滤器
			CAN_FilterInit(&CAN_FilterInitStructure);//	按上面的参数初始化过滤器
			printf("idh 	=0x%4x idl 	= 0x%04x\r\n",CAN_FilterInitStructure.CAN_FilterIdHigh,CAN_FilterInitStructure.CAN_FilterIdLow);
			printf("idMh 	=0x%4x idMl = 0x%04x\r\n",CAN_FilterInitStructure.CAN_FilterMaskIdHigh,CAN_FilterInitStructure.CAN_FilterMaskIdLow);
		}
}

void can_tx(u32 id ,u32 id_fmt,u8 *data,u8 len)
{ 
		CanTxMsg TxMessage;  

		TxMessage.StdId=id;	//标准标识符为0x00
		TxMessage.ExtId=id; //扩展标识符0x0000
		TxMessage.IDE=id_fmt;//使用标准标识符
		TxMessage.RTR=CAN_RTR_DATA;//为数据帧
		TxMessage.DLC=len;	//	消息的数据长度为2个字节
		memcpy(TxMessage.Data,data,len);
		CAN_Transmit(CAN1,&TxMessage); //发送数据 
}
/* USB中断和CAN接收中断服务程序，USB跟CAN公用I/O，这里只用到CAN的中断。 */
void USB_LP_CAN1_RX0_IRQHandler(void)
{
		OS_CPU_SR cpu_sr=0;	
		CanRxMsg RxMessage;
	
		OS_ENTER_CRITICAL();
	
		memset(&RxMessage,0,sizeof(CanRxMsg));  
		CAN_Receive(CAN1,CAN_FIFO0, &RxMessage); //接收FIFO0中的数据  
		resolve_can_message(&RxMessage);	
		OS_EXIT_CRITICAL();				
} 


