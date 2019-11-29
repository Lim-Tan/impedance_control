#include "encode.h"

u16	Parameter = 1024;

static void TIM4_Mode_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_ICInitTypeDef TIM_ICInitStructure;   	
  NVIC_InitTypeDef NVIC_InitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	
	
    GPIO_PinAFConfig(GPIOB,GPIO_PinSource6,GPIO_AF_TIM4);
    GPIO_PinAFConfig(GPIOB,GPIO_PinSource7,GPIO_AF_TIM4);
	
	/*- 正交编码器输入引脚 PB->6   PB->7 -*/

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
	

	/*- TIM4编码器模式配置 -*/
	TIM_DeInit(TIM4);
	TIM_TimeBaseStructure.TIM_Period = Parameter * 4-1;
	TIM_TimeBaseStructure.TIM_Prescaler = 0;
	TIM_TimeBaseStructure.TIM_ClockDivision =TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);              
                 
	//TIM_EncoderInterfaceConfig(TIM4, TIM_EncoderMode_TI1, TIM_ICPolarity_Rising ,TIM_ICPolarity_Rising);	//配置编码器模式触发源和极性
	TIM_EncoderInterfaceConfig(TIM4, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising ,TIM_ICPolarity_Rising);  //注意是TIM_ICPolarity_Rising
	
//	TIM_ICStructInit(&TIM_ICInitStructure);																																		//配置滤波器
//	TIM_ICInitStructure.TIM_ICFilter = 6;
//	TIM_ICInit(TIM4, &TIM_ICInitStructure);
//	
//	 TIM_ITConfig(  //使能或者失能指定的TIM中断
//		TIM4, //TIM2
//		TIM_IT_Update, //|
//		//TIM_IT_Trigger,   //TIM 触发中断源 
//		ENABLE  //使能
//		);
//    TIM_ClearFlag(TIM4, TIM_FLAG_Update|TIM_FLAG_Trigger); //清除溢出中断标志
//		NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;  //TIM3中断
//		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;  //先占优先级0级
//		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;  //从优先级3级
//		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
		
	TIM4->CNT = 2048;

	TIM_Cmd(TIM4, ENABLE);   //启动TIM4定时器
}


static void TIM8_Mode_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_ICInitTypeDef TIM_ICInitStructure;   	
   NVIC_InitTypeDef NVIC_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	
	
    GPIO_PinAFConfig(GPIOC,GPIO_PinSource6,GPIO_AF_TIM8);
    GPIO_PinAFConfig(GPIOC,GPIO_PinSource7,GPIO_AF_TIM8);
	
	/*- 正交编码器输入引脚 PC->6   PC->7 -*/

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
	

	/*- TIM4编码器模式配置 -*/
	TIM_DeInit(TIM8);
	TIM_TimeBaseStructure.TIM_Period = Parameter * 4-1;
	TIM_TimeBaseStructure.TIM_Prescaler = 0;
	TIM_TimeBaseStructure.TIM_ClockDivision =TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM8, &TIM_TimeBaseStructure);              
                 
	//TIM_EncoderInterfaceConfig(TIM8, TIM_EncoderMode_TI1, TIM_ICPolarity_Rising ,TIM_ICPolarity_Rising);	//配置编码器模式触发源和极性
	 TIM_EncoderInterfaceConfig(TIM8, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising ,TIM_ICPolarity_Rising);  //注意是TIM_ICPolarity_Rising
	
	TIM_ICStructInit(&TIM_ICInitStructure);																																		//配置滤波器
	TIM_ICInitStructure.TIM_ICFilter = 6;
	TIM_ICInit(TIM8, &TIM_ICInitStructure);
	
//		TIM_ITConfig(  //使能或者失能指定的TIM中断
//		TIM8, //TIM2
//		TIM_IT_Update  |  //TIM 中断源
//		TIM_IT_Trigger,   //TIM 触发中断源 
//		ENABLE  //使能
//		);
//TIM_ClearFlag(TIM8, TIM_FLAG_Update); //清除溢出中断标志
//		NVIC_InitStructure.NVIC_IRQChannel = TIM8_UP_TIM13_IRQn;  //TIM3中断
//		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;  //先占优先级0级
//		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;  //从优先级3级
//		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
//		NVIC_Init(&NVIC_InitStructure);  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器	
	
	TIM8->CNT = 2048;

	TIM_Cmd(TIM8, ENABLE);   //启动TIM4定时器
}

//static void TIM3_Mode_Config(void)
//{
//	GPIO_InitTypeDef GPIO_InitStructure;
//	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
//	TIM_ICInitTypeDef TIM_ICInitStructure;   	
//	NVIC_InitTypeDef NVIC_InitStructure;
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
//	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
//	
//	
//  GPIO_PinAFConfig(GPIOA,GPIO_PinSource6,GPIO_AF_TIM3);
//  GPIO_PinAFConfig(GPIOA,GPIO_PinSource7,GPIO_AF_TIM3);
//	
//	/*- 正交编码器输入引脚 PA->6   PA->7 -*/

//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
//    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
//    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
//    GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
//    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
//    GPIO_Init(GPIOA, &GPIO_InitStructure);
//	

//	/*- TIM3编码器模式配置 -*/
//	TIM_DeInit(TIM3);
//	TIM_TimeBaseStructure.TIM_Period = Parameter * 4-1;
//	TIM_TimeBaseStructure.TIM_Prescaler = 0;
//	TIM_TimeBaseStructure.TIM_ClockDivision =TIM_CKD_DIV1;
//	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
//	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);              
//                 
//	//TIM_EncoderInterfaceConfig(TIM8, TIM_EncoderMode_TI1, TIM_ICPolarity_Rising ,TIM_ICPolarity_Rising);	//配置编码器模式触发源和极性
//	 TIM_EncoderInterfaceConfig(TIM3, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising ,TIM_ICPolarity_Rising);  //注意是TIM_ICPolarity_Rising
//	
//	
//	TIM_ICStructInit(&TIM_ICInitStructure);																																		//配置滤波器
//	TIM_ICInitStructure.TIM_ICFilter = 6;
//	TIM_ICInit(TIM3, &TIM_ICInitStructure);
		
		
//	TIM_ITConfig(  //使能或者失能指定的TIM中断
//		TIM3, //TIM3
//		TIM_IT_Update  |  //TIM 中断源
//		TIM_IT_Trigger,   //TIM 触发中断源 
//		ENABLE  //使能
//		);
//    TIM_ClearFlag(TIM3, TIM_FLAG_Update); //清除溢出中断标志
//		NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM3中断
//		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;  //先占优先级0级
//		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;  //从优先级3级
//		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
//		NVIC_Init(&NVIC_InitStructure);  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器
		
//	TIM3->CNT = 0;  //0

//	TIM_Cmd(TIM3, ENABLE);   //启动TIM3定时器
//}

int TIM4_num=0;
//定时采集数据
void TIM4_IRQHandler(void)   //TIM4中断
{
	if (TIM_GetITStatus(TIM4, TIM_IT_Update) == SET) //检查指定的TIM中断发生与否:TIM 中断源 
	{
		if((TIM4->CR1&TIM_CR1_DIR)==0)TIM4_num++;
		else TIM4_num--;
		TIM_ClearITPendingBit(TIM4, TIM_IT_Update);  //清除TIMx的中断待处理位:TIM 中断源 
		//modbus_rtu();
	}
}

int TIM8_num=0;
//定时采集数据
void TIM8_UP_TIM13_IRQHandler(void)   //TIM4中断
{
	if (TIM_GetITStatus(TIM8, TIM_IT_Update) == SET) //检查指定的TIM中断发生与否:TIM 中断源 
	{
		if((TIM8->CR1&TIM_CR1_DIR)==0)TIM8_num++;
		else TIM8_num--;
		TIM_ClearITPendingBit(TIM8, TIM_IT_Update);  //清除TIMx的中断待处理位:TIM 中断源 
		//modbus_rtu();
	}
}

//int TIM3_num=0;
////定时采集数据
//void TIM3_IRQHandler(void)   //TIM4中断
//{
//	if (TIM_GetITStatus(TIM3, TIM_IT_Update) == SET) //检查指定的TIM中断发生与否:TIM 中断源 
//	{
//		if((TIM3->CR1&TIM_CR1_DIR)==0) TIM3_num++;
//		else TIM3_num--;
//		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);  //清除TIMx的中断待处理位:TIM 中断源 
//		//modbus_rtu();
//	}
//}

s32 GetEncoder1(void)
{
	s32 count=TIM4->CNT;
	return count+TIM4_num*4096;
}

s32 GetEncoder2(void)
{
	s32 count=TIM8->CNT;
	return count+TIM8_num*4096;
}


//s32 GetEncoder3(void)
//{
//	s32 count=TIM3->CNT;
//	return count+TIM3_num*4096;
//}

void encode_Init(void)
{
  TIM4_Mode_Config();
  TIM8_Mode_Config();
//	TIM3_Mode_Config();
}

