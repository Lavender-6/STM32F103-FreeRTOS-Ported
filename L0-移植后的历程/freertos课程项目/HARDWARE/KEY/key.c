#include "stm32f10x.h"
#include "key.h"
#include "sys.h" 
#include "delay.h"

/*
	信盈达STM32-V4 按键口线分配：
		K1 键         : PA0   (高电平表示按下)
		K2 键         : PC4   (低电平表示按下)
		K3 键         : PC5   (低电平表示按下)
		K4 键         : PC6   (低电平表示按下)

*/

/* 按键口对应的RCC时钟 */
#define RCC_ALL_KEY 	(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOC )

#define GPIO_PORT_K1    GPIOA
#define GPIO_PIN_K1	    GPIO_Pin_0

#define GPIO_PORT_K2    GPIOC
#define GPIO_PIN_K2	    GPIO_Pin_4

#define GPIO_PORT_K3    GPIOC
#define GPIO_PIN_K3	    GPIO_Pin_5

#define GPIO_PORT_K4    GPIOC
#define GPIO_PIN_K4	    GPIO_Pin_6





//按键和摇杆初始化函数
void KEY_Init(void) //IO初始化
{ 
 	GPIO_InitTypeDef GPIO_InitStructure;

	/* 第1步：打开GPIO时钟 */
	RCC_APB2PeriphClockCmd(RCC_ALL_KEY, ENABLE);

	/* 第2步：配置所有的按键GPIO为浮动输入模式(实际上CPU复位后就是输入状态) */
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	/* 输入浮空模式 */
	
	GPIO_InitStructure.GPIO_Pin = GPIO_PIN_K1;
	GPIO_Init(GPIO_PORT_K1, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_PIN_K2;
	GPIO_Init(GPIO_PORT_K2, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_PIN_K3;
	GPIO_Init(GPIO_PORT_K3, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_PIN_K4;
	GPIO_Init(GPIO_PORT_K4, &GPIO_InitStructure);


}
//按键处理函数
//返回按键值
//mode:0,不支持连续按;1,支持连续按;
//0，没有任何按键按下
//1，KEY1按下
//2，KEY2按下
//3，KEY3按下 
//4，KEY4按下 
//注意此函数有响应优先级,KEY1>KEY2>KEY3>KEY4>KEY5>KEY6>KEY7>KEY8!!
u8 KEY_Scan(u8 mode)
{	 
	static u8 key_up=1;//按键按松开标志
	if(mode)key_up=1;  //支持连按		  
	if(key_up&&(KEY1==1||KEY2==0||KEY3==0||KEY4==0))
	{
		delay_ms(10);//去抖动 
		key_up=0;
		if(KEY1==1)return KEY1_PRES;
		else if(KEY2==0)return KEY2_PRES;
		else if(KEY3==0)return KEY3_PRES;
		else if(KEY4==0)return KEY4_PRES;
		
	}else if(KEY1==0&&KEY2==1&&KEY3==1&&KEY4==1)key_up=1; 	    
 	return 0;// 无按键按下
}
