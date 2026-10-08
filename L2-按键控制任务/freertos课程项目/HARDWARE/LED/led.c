#include "led.h"

 	   

//初始化PE2-5为输出口.并使能这个口的时钟		    
//LED IO初始化
void LED_Init(void)
{
 
 GPIO_InitTypeDef  GPIO_InitStructure;
 	
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);	 //使能PE端口时钟
	
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;				 //LED1-->PE.2 端口配置
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
 GPIO_Init(GPIOE, &GPIO_InitStructure);					 //根据设定参数初始化GPIOE.2
 GPIO_SetBits(GPIOF,GPIO_Pin_2);						 //PE.2 输出高

 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;	    		 //LED2-->PE.3 端口配置, 推挽输出
 GPIO_Init(GPIOE, &GPIO_InitStructure);	  				 //推挽输出 ，IO口速度为50MHz
 GPIO_SetBits(GPIOE,GPIO_Pin_3); 						 //PE.3 输出高 
	
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;				 //LED3-->PE.4 端口配置
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
 GPIO_Init(GPIOE, &GPIO_InitStructure);					 //根据设定参数初始化GPIOE.4
 GPIO_SetBits(GPIOE,GPIO_Pin_4);						 //PE.4 输出高

 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;				 //LED4-->PE.5 端口配置
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
 GPIO_Init(GPIOE, &GPIO_InitStructure);					 //根据设定参数初始化GPIOE.5
 GPIO_SetBits(GPIOE,GPIO_Pin_5);						 //PE.5输出高
}
 
