#include "lcd.h"
#include "delay.h"
#include "stdio.h"

void LCD_WR_REG(volatile uint16_t regval)
{
  regval=regval;		//使用-O2优化的时候,必须插入的延时
  TFTLCD->LCD_REG=regval;//写入要写的寄存器序号
}
//写LCD数据
//data:要写入的值
void LCD_WR_DATA(volatile uint16_t data)
{
  data=data;			//使用-O2优化的时候,必须插入的延时
  TFTLCD->LCD_RAM=data;
}
//读LCD数据
//返回值:读到的值
uint16_t LCD_RD_DATA(void)
{
  volatile uint16_t ram;			//防止被优化
  ram=TFTLCD->LCD_RAM;
  return ram;
}
//写寄存器
//LCD_Reg:寄存器地址
//LCD_RegValue:要写入的数据
void LCD_WriteReg(uint16_t LCD_Reg,uint16_t LCD_RegValue)
{
  TFTLCD->LCD_REG = LCD_Reg;		//写入要写的寄存器序号
  TFTLCD->LCD_RAM = LCD_RegValue;//写入数据
}
//读寄存器
//LCD_Reg:寄存器地址
//返回值:读到的数据
uint16_t LCD_ReadReg(uint16_t LCD_Reg)
{
  LCD_WR_REG(LCD_Reg);		//写入要读的寄存器序号
  delay_us(5);
  return LCD_RD_DATA();		//返回读到的值
}

//LCD写GRAM
//RGB_Code:颜色值
void LCD_WriteRAM(uint16_t RGB_Code)
{
  TFTLCD->LCD_RAM = RGB_Code;//写十六位GRAM
}



void Init_LCD(void)
{
	u32 i=0;
	//1：开时钟
	//2：配置LCD连接IO口（复用推挽）
	FSMC_NORSRAMInitTypeDef FSMC_NORSRAMInitStruct;
	FSMC_NORSRAMTimingInitTypeDef FSMC_NORSRAMTimingInitWirte;
	FSMC_NORSRAMTimingInitTypeDef FSMC_NORSRAMTimingInitRead;
	
	GPIO_InitTypeDef GPIO_InitStructure;
	

	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_FSMC,ENABLE);	//使能FSMC时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOD|RCC_APB2Periph_GPIOE|RCC_APB2Periph_GPIOG|RCC_APB2Periph_AFIO,ENABLE);//使能PORTB,D,E,G以及AFIO复用功能时钟

 	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;				 //PB0 推挽输出 背光
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOB, &GPIO_InitStructure);

 	//PORTD复用推挽输出  
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_14|GPIO_Pin_15;				 //	//PORTD复用推挽输出  
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 		 //复用推挽输出   
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOD, &GPIO_InitStructure); 
	  
	//PORTE复用推挽输出  
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;				 //	//PORTD复用推挽输出  
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 		 //复用推挽输出   
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOE, &GPIO_InitStructure); 
	  
   	//	//PORTG12复用推挽输出 A10	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_12;	 //	//PORTD复用推挽输出  
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 		 //复用推挽输出   
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOG, &GPIO_InitStructure);


	FSMC_NORSRAMTimingInitRead.FSMC_AddressSetupTime = 0x01;	 //地址建立时间（ADDSET）为2个HCLK 1/36M=27ns
	FSMC_NORSRAMTimingInitRead.FSMC_AddressHoldTime = 0x00;	 //地址保持时间（ADDHLD）模式A未用到	
	FSMC_NORSRAMTimingInitRead.FSMC_DataSetupTime = 0x0f;		 // 数据保存时间为16个HCLK,因为液晶驱动IC的读数据的时候，速度不能太快，尤其对1289这个IC。
	FSMC_NORSRAMTimingInitRead.FSMC_BusTurnAroundDuration = 0x00;
	FSMC_NORSRAMTimingInitRead.FSMC_CLKDivision = 0x00;
	FSMC_NORSRAMTimingInitRead.FSMC_DataLatency = 0x00;
	FSMC_NORSRAMTimingInitRead.FSMC_AccessMode = FSMC_AccessMode_A;	 //模式A 
    

	FSMC_NORSRAMTimingInitWirte.FSMC_AddressSetupTime = 0x00;	 //地址建立时间（ADDSET）为1个HCLK  
	FSMC_NORSRAMTimingInitWirte.FSMC_AddressHoldTime = 0x00;	 //地址保持时间（A		
	FSMC_NORSRAMTimingInitWirte.FSMC_DataSetupTime = 0x03;		 ////数据保存时间为4个HCLK	
	FSMC_NORSRAMTimingInitWirte.FSMC_BusTurnAroundDuration = 0x00;
	FSMC_NORSRAMTimingInitWirte.FSMC_CLKDivision = 0x00;
	FSMC_NORSRAMTimingInitWirte.FSMC_DataLatency = 0x00;
	FSMC_NORSRAMTimingInitWirte.FSMC_AccessMode = FSMC_AccessMode_A;	 //模式A 
/*
	FSMC_NORSRAMInitStruct.FSMC_AsynchronousWait=FSMC_AsynchronousWait_Disable;
	FSMC_NORSRAMInitStruct.FSMC_Bank=FSMC_Bank1_NORSRAM4;
	FSMC_NORSRAMInitStruct.FSMC_BurstAccessMode=FSMC_BurstAccessMode_Disable;
	FSMC_NORSRAMInitStruct.FSMC_DataAddressMux=FSMC_DataAddressMux_Disable;
	FSMC_NORSRAMInitStruct.FSMC_ExtendedMode=FSMC_ExtendedMode_Enable;
	FSMC_NORSRAMInitStruct.FSMC_MemoryDataWidth=FSMC_MemoryDataWidth_16b;
	FSMC_NORSRAMInitStruct.FSMC_MemoryType=FSMC_MemoryType_SRAM;
	FSMC_NORSRAMInitStruct.FSMC_WriteOperation =FSMC_WriteOperation_Enable;
*/	
	FSMC_NORSRAMInitStruct.FSMC_Bank = FSMC_Bank1_NORSRAM4;//  这里我们使用NE4 ，也就对应BTCR[6],[7]。
	FSMC_NORSRAMInitStruct.FSMC_DataAddressMux = FSMC_DataAddressMux_Disable; // 不复用数据地址
	FSMC_NORSRAMInitStruct.FSMC_MemoryType =FSMC_MemoryType_SRAM;// FSMC_MemoryType_SRAM;  //SRAM   
	FSMC_NORSRAMInitStruct.FSMC_MemoryDataWidth = FSMC_MemoryDataWidth_16b;//存储器数据宽度为16bit   
	FSMC_NORSRAMInitStruct.FSMC_BurstAccessMode =FSMC_BurstAccessMode_Disable;// FSMC_BurstAccessMode_Disable; 
	FSMC_NORSRAMInitStruct.FSMC_WaitSignalPolarity = FSMC_WaitSignalPolarity_Low;
	FSMC_NORSRAMInitStruct.FSMC_AsynchronousWait=FSMC_AsynchronousWait_Disable; 
	FSMC_NORSRAMInitStruct.FSMC_WrapMode = FSMC_WrapMode_Disable;   
	FSMC_NORSRAMInitStruct.FSMC_WaitSignalActive = FSMC_WaitSignalActive_BeforeWaitState;  
	FSMC_NORSRAMInitStruct.FSMC_WriteOperation = FSMC_WriteOperation_Enable;	//  存储器写使能
	FSMC_NORSRAMInitStruct.FSMC_WaitSignal = FSMC_WaitSignal_Disable;   
	FSMC_NORSRAMInitStruct.FSMC_ExtendedMode = FSMC_ExtendedMode_Enable; // 读写使用不同的时序
	FSMC_NORSRAMInitStruct.FSMC_WriteBurst = FSMC_WriteBurst_Disable; 
	
	
	FSMC_NORSRAMInitStruct.FSMC_ReadWriteTimingStruct=&FSMC_NORSRAMTimingInitRead;
	FSMC_NORSRAMInitStruct.FSMC_WriteTimingStruct=&FSMC_NORSRAMTimingInitWirte;
	FSMC_NORSRAMInit(&FSMC_NORSRAMInitStruct);
	FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM4,ENABLE);
	delay_ms(100);
	LCD_WR_REG(0xCF);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0xC1);
    LCD_WR_DATA(0X30);
    LCD_WR_REG(0xED);
    LCD_WR_DATA(0x64);
    LCD_WR_DATA(0x03);
    LCD_WR_DATA(0X12);
    LCD_WR_DATA(0X81);
    LCD_WR_REG(0xE8);
    LCD_WR_DATA(0x85);
    LCD_WR_DATA(0x10);
    LCD_WR_DATA(0x7A);
    LCD_WR_REG(0xCB);
    LCD_WR_DATA(0x39);
    LCD_WR_DATA(0x2C);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x34);
    LCD_WR_DATA(0x02);
    LCD_WR_REG(0xF7);
    LCD_WR_DATA(0x20);
    LCD_WR_REG(0xEA);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x00);
    LCD_WR_REG(0xC0);    //Power control
    LCD_WR_DATA(0x1B);   //VRH[5:0]
    LCD_WR_REG(0xC1);    //Power control
    LCD_WR_DATA(0x01);   //SAP[2:0];BT[3:0]
    LCD_WR_REG(0xC5);    //VCM control
    LCD_WR_DATA(0x30); 	 //3F
    LCD_WR_DATA(0x30); 	 //3C
    LCD_WR_REG(0xC7);    //VCM control2
    LCD_WR_DATA(0XB7);
    LCD_WR_REG(0x36);    // Memory Access Control
    LCD_WR_DATA(0x48);
    LCD_WR_REG(0x3A);
    LCD_WR_DATA(0x55);
    LCD_WR_REG(0xB1);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x1A);
    LCD_WR_REG(0xB6);    // Display Function Control
    LCD_WR_DATA(0x0A);
    LCD_WR_DATA(0xA2);
    LCD_WR_REG(0xF2);    // 3Gamma Function Disable
    LCD_WR_DATA(0x00);
    LCD_WR_REG(0x26);    //Gamma curve selected
    LCD_WR_DATA(0x01);
    LCD_WR_REG(0xE0);    //Set Gamma
    LCD_WR_DATA(0x0F);
    LCD_WR_DATA(0x2A);
    LCD_WR_DATA(0x28);
    LCD_WR_DATA(0x08);
    LCD_WR_DATA(0x0E);
    LCD_WR_DATA(0x08);
    LCD_WR_DATA(0x54);
    LCD_WR_DATA(0XA9);
    LCD_WR_DATA(0x43);
    LCD_WR_DATA(0x0A);
    LCD_WR_DATA(0x0F);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x00);
    LCD_WR_REG(0XE1);    //Set Gamma
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x15);
    LCD_WR_DATA(0x17);
    LCD_WR_DATA(0x07);
    LCD_WR_DATA(0x11);
    LCD_WR_DATA(0x06);
    LCD_WR_DATA(0x2B);
    LCD_WR_DATA(0x56);
    LCD_WR_DATA(0x3C);
    LCD_WR_DATA(0x05);
    LCD_WR_DATA(0x10);
    LCD_WR_DATA(0x0F);
    LCD_WR_DATA(0x3F);
    LCD_WR_DATA(0x3F);
    LCD_WR_DATA(0x0F);
    LCD_WR_REG(0x2B);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x01);
    LCD_WR_DATA(0x3f);
    LCD_WR_REG(0x2A);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0xef);
    LCD_WR_REG(0x11); //Exit Sleep
    delay_ms(120);
    LCD_WR_REG(0x29); //display on
	
	
	LCD_WR_REG(0x36);
	LCD_WR_DATA(0x08);//II9341必须这样
	
	//整个LCD就初始化完成
	//lCD有个背光灯
	LCD_LED_ON;//打开LCD屏幕背光灯
	//REG :写命令
	//DATA:写数据
	LCD_WR_REG(0x2A);
	LCD_WR_DATA(0);
	LCD_WR_DATA(0);  //
	LCD_WR_DATA(0);
	LCD_WR_DATA((240-1));
//X轴是从 0开始   到240结束
	LCD_WR_REG(0x2B);
	LCD_WR_DATA(0);
	LCD_WR_DATA(0);
	LCD_WR_DATA((320-1)>>8);
	LCD_WR_DATA((320-1)&0xFF);
//y轴的起始 0
//y轴的结束 320
	LCD_WR_REG(0x2C);
	for(i=0;i<240*320;i++)
	{
		LCD_WR_DATA(0x7E0);//绿色
		
	}
		
}


void Clear_LCD(u16 Color)
{
	u32 i=0;
	LCD_WR_REG(0x2A);
	LCD_WR_DATA(0);
	LCD_WR_DATA(0);  //
	LCD_WR_DATA(0);
	LCD_WR_DATA((240-1));
	LCD_WR_REG(0x2B);
	LCD_WR_DATA(0);
	LCD_WR_DATA(0);
	LCD_WR_DATA((320-1)>>8);
	LCD_WR_DATA((320-1)&0xFF);

	LCD_WR_REG(0x2C);
	for(i=0;i<240*320;i++)
	{
		LCD_WR_DATA(Color);//绿色
	}
}



void show_pic(u16 x,u16 y,u16 w,u16 h,u8 * pic)
{
	u32 i =0;
	LCD_WR_REG(0x2A);
	LCD_WR_DATA((x)>>8);
	LCD_WR_DATA((x)&0xFF);  
	LCD_WR_DATA((w-1)>>8);
	LCD_WR_DATA((w-1)&0xFF); 
	LCD_WR_REG(0x2B);
	LCD_WR_DATA((y)>>8);
	LCD_WR_DATA((y)&0xFF);
	LCD_WR_DATA((h-1)>>8);
	LCD_WR_DATA((h-1)&0xFF);
	//从哪里开始画图片
	LCD_WR_REG(0x2C);
	for(i=0;i<w*h*2;i++)
	{
		LCD_WR_DATA(pic[i]<<8|pic[i+1]);
		i++;
	}
}

//
void Draw_point(u16 x,u16 y,u16 color)
{
	LCD_WR_REG(0x2A);
	LCD_WR_DATA(x>>8);//320高八位
	LCD_WR_DATA(x&0xFF); //320的低八位
	
	LCD_WR_REG(0x2B);
	LCD_WR_DATA(y>>8);
	LCD_WR_DATA(y&0xFF); 
	LCD_WR_REG(0x2C);
	LCD_WR_DATA(color);
}

void Draw_xline(void)
{
	u16 i=0;
	for(i=0;i<240;i++)
	{
		Draw_point(i,160,0xF800);
	}
	for(i=0;i<240;i++)
	{
		Draw_point(i,161,0xF800);
	}
	for(i=0;i<240;i++)
	{
		Draw_point(i,162,0xF800);
	}for(i=0;i<240;i++)
	{
		Draw_point(i,163,0xF800);
	}
}

char font_ping[]=
{
	0x00,0x00,0x7F,0xFC,0x01,0x00,0x01,0x00,0x11,0x10,0x09,0x10,0x09,0x20,0x01,0x00,
	0xFF,0xFE,0x01,0x00,0x01,0x00,0x01,0x00,0x01,0x00,0x01,0x00,0x01,0x00,0x01,0x00,/*"平",0*/
};
//写字函数
//16*16汉字占几个字节
//16*16/8=32
void Wirte_ping_font(void)
{
	u8 i,j;
	u16 x0=0,y0=0;
	for(i=0;i<32;i++)
	{
		 for(j=0;j<8;j++)
		{
			
			if((font_ping[i]<<j)&0x80)
			{
				Draw_point(x0,y0,0xF800);
			}else
			{
				Draw_point(x0,y0,0x0000);
			}
			x0++;
		}
		if(i%2==1)
		{
			x0=0;
			y0++;
		}
	}
}
//显示32*32的汉字
u8 font_32[128]={0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x80,0x00,0x00,0x01,0xE0,0x00,0x00,0x01,0xC0,0x00,0x30,0x01,0xC1,0xFF,0xF8,0x01,0xC0,0x00,0x78,0x01,0x80,0x00,0xE0,
0x03,0x8C,0x00,0xC0,0x3F,0xFE,0x01,0x80,0x03,0x8C,0x07,0x00,0x03,0x8C,0x07,0x00,0x03,0x1C,0x07,0x00,0x07,0x1C,0x07,0x00,0x07,0x1C,0x07,0x00,0x06,0x1C,0x07,0x1C,
0x06,0x1F,0xFF,0xFE,0x0E,0x38,0x07,0x00,0x0E,0x38,0x07,0x00,0x0C,0x38,0x07,0x00,0x0C,0x70,0x07,0x00,0x0F,0x70,0x07,0x00,0x01,0xE0,0x07,0x00,0x00,0xF8,0x07,0x00,
0x00,0xFE,0x07,0x00,0x01,0xCE,0x07,0x00,0x03,0x86,0x07,0x00,0x06,0x00,0x07,0x00,0x0C,0x00,0xFE,0x00,0x38,0x00,0x1E,0x00,0x00,0x00,0x0C,0x00,0x00,0x00,0x00,0x00,/*"好",0*/
/* (32 X 32 , 宋体, 加粗 )*/

}; 
// 字体函数改一下
// u8 * font 这个参数改成直接传字的形式显示出来
void Wirte_32_font(u16 x,u16 y,u16 backclock,u16 fontcolock,u8 * font)
{
	u8 i,j;
	u16 x0=x,y0=y;
	for(i=0;i<128;i++)
	{
		 for(j=0;j<8;j++)
		{
			
			if((font[i]<<j)&0x80)
			{
				Draw_point(x0,y0,fontcolock);
			}else
			{
				Draw_point(x0,y0,backclock);
			}
			x0++;
		}
		if(i%4==3)
		{
			x0=x;
			y0++;
		}
	}
}


u8 H_Font[]={
0x00,0x00,0x00,0xE7,0x42,0x42,0x42,0x42,0x7E,0x42,0x42,0x42,0x42,0xE7,0x00,0x00,/*"H",0*/
/* (8 X 16 , 宋体 )*/
};
void show_H(u16 x,u16 y,u16 backclock,u16 fontcolock)
{
	u8 i,j;
	u16 x0=x,y0=y;
//16*16 汉字  16*16/8
//16*8  ASCII 16*8/8
	for(i=0;i<16;i++)
	{
		 for(j=0;j<8;j++)
		{
			
			if((H_Font[i]<<j)&0x80)
			{
				Draw_point(x0,y0,fontcolock);
			}else
			{
				Draw_point(x0,y0,backclock);
			}
			x0++;
		}
		x0=x;
		y0++;
	}
}



u8 * find_font(u8 font[2])
{
	u16 i=0;
	while(1)
	{
		if(font[0]==font_lib[i]&&font[1]==font_lib[i+1])
		{	
			return &(font_data[i*16]);
		}else
		{
			i+=2;
		}
	}
}
void show_16_one_font(u16 x,u16 y,u16 backclock,u16 fontcolock,u8 font[2])
{
	u8 i,j;
	u16 x0=x,y0=y;
	u8 * p=NULL;
	p=find_font(font);
	for(i=0;i<32;i++)
	{
		printf("%x\r\n",p[i]);
	}
	for(i=0;i<32;i++)
	{
		 for(j=0;j<8;j++)
		{
			
			if((p[i]<<j)&0x80)
			{
				Draw_point(x0,y0,fontcolock);
			}else
			{
				Draw_point(x0,y0,backclock);
			}
			x0++;
		}
		if(i%2==1)
		{
			x0=x;
			y0++;
		}
	}
}



void Show_font_16(u16 x,u16 y,u16 backclock,u16 fontcolock,u8 * font)
{
	u16 x0,y0;
	u8 * font_tmp;
	x0=x;y0=y;
	font_tmp = font;
	while((*font_tmp)!='\0')
	{
		show_16_one_font(x0,y0,backclock,fontcolock,font_tmp);
		x0+=16;
		if(x0>=239)
		{
			y0+=16;
			x0=0;
		}
		font_tmp++;
		if((*font_tmp)==0)
		{
			return ;
		}
		font_tmp++;
	}
}


uint8_t Font_ZfFind(char ch)
{
	//uint32_t lenth = sizeof((uint8_t *)zf_32)-1;//求出字符集的长度
	uint32_t i = 0;//
	for(i=0;i<255;i++)
	{
		if(zf_32[i] == ch)
		return i;
	}
	return 0xff;
}

void Show_One_ASIIC(uint16_t x, uint16_t y,uint16_t font_color,uint16_t bk_color,uint8_t asc)
{
	uint8_t i,j;
	uint8_t offset;
	uint8_t x1=0,y1=0;//从哪个位置开始打点
	x1 = x , y1=y;
	offset = 0;
	offset=Font_ZfFind(asc);//查找我们字符所在字库的位置
	printf("offset=====%d\r\n",offset);
	//offset+=2;
	for(i=0;i<16;i++)
	{
		for(j=0;j<8;j++)   
		{
			if((AscII_32_32_data[offset][i]<<j) & 0x80)//这个是在判断这个字节的最高位是否为1
			{
				Draw_point(x1,y1,font_color);//在这个位置打上一个红色点
			}else
			{
				Draw_point(x1,y1,bk_color);//在这个位置打上一个黑色点
			}
			x1++;
		}
		
		x1=x;
		y1+=1;
	}
}


void Show_ASCII_32(uint16_t x, uint16_t y,uint16_t font_color,uint16_t bk_color,uint8_t * font)
{
	while(* font!=0)
	{
		Show_One_ASIIC(x,y,bk_color,font_color,* font);
		x+=8;
		font+=1;
		if(x>=240)
		{
			x=0;
			y+=16;//
		}
	}
}
//直接拿着
void Show_Sting(u16 x,u16 y,u16 backclock,u16 fontcolock,u8 * font)
{
	while(* font!=0)
	{
		if(* font>=0xa1) //如果第一个字节大于0xA0
		{
			show_16_one_font(x,y,backclock,fontcolock,font);
			x+=16;
			font+=2;//字体占两个字节
			if(x>=240)
			{
				x=0;
				y+=16;//
			}//汉字
		}else // 肯定是ASCII
		{
			Show_One_ASIIC(x,y,fontcolock,backclock,* font);
			x+=8;
			font+=1; //ASCII占一个字节
			if(x>=240)
			{
				x=0;
				y+=16;//
			}
		}
	}
}

