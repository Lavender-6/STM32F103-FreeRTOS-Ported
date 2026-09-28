#include "exti.h"
#include "led.h"
#include "key.h"
#include "exti.h"
#include "delay.h"
//#include "FreeRTOS.h"
//#include "task.h"


//外部中断0服务程序
void EXTIX_Init(void)
{
 
 	EXTI_InitTypeDef EXTI_InitStructure;
 	NVIC_InitTypeDef NVIC_InitStructure;

    //KEY_Init();	 //	按键端口初始化

  	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);	//使能复用功能时钟

   //GPIOA.0  中断线以及中断初始化配置  下降沿触发	//KEY_RIGHT
  	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource0);

	  EXTI_InitStructure.EXTI_Line=EXTI_Line0;
  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
  	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  	EXTI_Init(&EXTI_InitStructure);	 	//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器

	  NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;				//使能按键KEY_LEFT所在的外部中断通道
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x06;	//抢占优先级6
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;			//子优先级0 
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;					//使能外部中断通道
  	NVIC_Init(&NVIC_InitStructure);  	  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器
 
}

        

//外部中断15_10服务程序
void EXTI0_IRQHandler(void)
{
	
	delay_ms(20);	//消抖
	if(KEY1==0)	 
	{				 
		LED1 = ~ LED1;
	}		 
	 EXTI_ClearITPendingBit(EXTI_Line0);//清除LINE14上的中断标志位  
}
