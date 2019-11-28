
/*********************************************************************************************************
*	                                  
*	模块名称 : AD7606驱动模块
*	文件名称 : bsp.c
*	版    本 : V1.1
*	说    明 : 驱动AD7606 ADC转换器 SPI接口
*
*
*********************************************************************************************************/


#include "bsp.h"
#include "delay.h"
#include "stm32f4xx.h"
#include <rtthread.h>

void bsp_InitAD7606(void)
{
	 GPIO_InitTypeDef  GPIO_InitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);//使能GPID时钟

  //GPIOD0~7,E0初始化设置
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3|GPIO_Pin_6|GPIO_Pin_12|GPIO_Pin_11;//输出接口的定义
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
  GPIO_Init(GPIOD, &GPIO_InitStructure);//初始化GPIO		
	

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);//使能GPIC时钟
	  //GPIOC0~3,E0初始化设置
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3;//输出接口的定义
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
  GPIO_Init(GPIOC, &GPIO_InitStructure);//初始化GPIO	

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);//使能GPIOE时钟
 
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0; //MISO对应引脚  串行输入输出口
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//普通输入模式
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100M
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
  GPIO_Init(GPIOE, &GPIO_InitStructure);//初始化GPIOE0
	
	GPIO_SetBits(GPIOD,GPIO_Pin_3);  //设置采样范围 为 10v
}

/*
*********************************************************************************************************
*	函 数 名: ad7606_Reset
*	功能说明: 硬件复位AD7606
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void ad7606_Reset(void)
{
	/* AD7606是高电平复位，要求最小脉宽50ns */
	
	AD_RESET_LOW();
	
	AD_RESET_HIGH();
	AD_RESET_HIGH();
	AD_RESET_HIGH();
	AD_RESET_HIGH();
	
	AD_RESET_LOW();
}

/*
*********************************************************************************************************
*	函 数 名: ad7606_SetOS
*	功能说明: 设置过采样模式（数字滤波，硬件求平均值)
*	形    参：_ucMode : 0-6  0表示无过采样，1表示2倍，2表示4倍，3表示8倍，4表示16倍
*				5表示32倍，6表示64倍
*	返 回 值: 无
*********************************************************************************************************/
void AD7606_SetOS(u16 ucOS)
{
	switch (ucOS)
	{
		case 2:
			OS2_0();
			OS1_0();
			OS0_1();
			break;

		case 4:
			OS2_0();
			OS1_1();
			OS0_0();
			break;

		case 8:
			OS2_0();
			OS1_1();
			OS0_1();
			break;

		case 16:
			OS2_1();
			OS1_0();
			OS0_0();
			break;

		case 32:
			OS2_1();
			OS1_0();
			OS0_1();
			break;

		case 64:
			OS2_1();
			OS1_1();
			OS0_0();
			break;

		case 1:
		default:
			OS2_0();
			OS1_0();
			OS0_0();
			break;
	}
}

/*
*********************************************************************************************************
*	函 数 名: ad7606_StartConv
*	功能说明: 启动AD7606的ADC转换
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void ad7606_StartConv(void)
{
	/* 上升沿开始转换，低电平持续时间至少25ns  */
	AD_CONVST_LOW();
	AD_CONVST_LOW();
	AD_CONVST_LOW();	/* 连续执行2次，低电平约50ns */
	AD_CONVST_LOW();
	AD_CONVST_LOW();
	AD_CONVST_LOW();
	AD_CONVST_LOW();
	AD_CONVST_LOW();
	AD_CONVST_LOW();
	
	AD_CONVST_HIGH();
}



s16  ReceiveData(void)	  
{ 	 
	u8 count=0; 	  
	s16 Num=0; 
	AD_CS_HIGH();
  AD_CLK_HIGH();	
	AD_CS_LOW();
	for(count=0;count<16;count++)//读出16位数据
	{
		AD_CLK_LOW();	//下降沿有效
		delay_us(1);
		if(DATA)
		Num|=0x01;
		AD_CLK_HIGH();
		Num<<=1;
		
		AD_CLK_HIGH();
	}
	return  Num;
}





