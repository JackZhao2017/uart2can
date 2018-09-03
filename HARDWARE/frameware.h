#ifndef _FRAMEWARE_H_
#define _FRAMEWARE_H_


#define  CAN_IRQPriority      0
#define  CAN_IRQSubPriority   1


#define  TIM4_IRQPriority     0
#define  TIM4_IRQSubPriority  2

#define  UART2_IRQPriority    2
#define  UART2_IRQSubPriority 3

#define  UART_IRQPriority     3
#define  UART_IRQSubPriority  3


#define  EXTI_IRQPriority     1
#define  EXTI_IRQSubPriority1 1
#define  EXTI_IRQSubPriority2 2



void FRAMEWORK_Init(void);
void SYSCLKConfig_STOP(void);
void FRAMEWORK_Release(void);
#endif
