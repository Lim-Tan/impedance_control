#include "timer1.h"
//#include "led.h"
#include "master.h"
#include "modbus.h"

extern u8 modbus_com2_over;



//通用定时器中断初始化	通用定时器 2、3、4
//这里时钟选择为APB1的2倍，而APB1为36M
//arr：自动重装值。
//psc：时钟预分频数
//这里使用的是定时器2,3!
//Tout=((arr+1)*(psc+1))/Ft us.
//Ft:Mhz
//TIM1和TIM8~TIM11使用APB2时钟频率168M,TIM2~TIM7和TIM12~TIM14使用APB1时钟频率为84M

void Timer3_Init(u16 arr,u16 psc)
{
		TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
		NVIC_InitTypeDef NVIC_InitStructure;

		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //时钟使能	  
//	TIM_TimeBaseStructure.TIM_Period = 5000; //定时器时钟84M,分频系数8400,所以84M/8400=10Khz的计数频率,计数5000次为500ms
//	TIM_TimeBaseStructure.TIM_Prescaler =(7200-1); //设置用来作为TIMx时钟频率除数的预分频值  10Khz的计数频率
		TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	 计数到5000为500ms
		TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值  10Khz的计数频率
		TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
		TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
		TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
 
//	TIM_ITConfig(  //使能或者失能指定的TIM中断
//		TIM3, //TIM3
//		TIM_IT_Update  |  //TIM 中断源
//		TIM_IT_Trigger,   //TIM 触发中断源 
//		ENABLE  //使能
//		);
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;  //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器

	TIM_ClearFlag(TIM3, TIM_FLAG_Update); //清除溢出中断标志

//	TIM_Cmd(TIM3, ENABLE);  //使能TIMx外设
							 
}

void Timer3_enable( void )
{
		TIM_ClearFlag(TIM3, TIM_FLAG_Update); //清除溢出中断标志
		TIM_SetCounter(TIM3,0x00);			//清零计数器值
		TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);
		TIM_Cmd(TIM3,ENABLE);
}

void Timer3_disable (void)
{
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, DISABLE); //时钟失能
//	TIM_ITConfig(TIM2,TIM_IT_Update,DISABLE);
		TIM_ITConfig(TIM3, TIM_IT_Update | TIM_IT_Trigger,DISABLE );
		TIM_Cmd(TIM3, DISABLE);  //失能TIMx外设
}


void Timer2_Init(u16 arr, u16 psc)
{
		TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
		NVIC_InitTypeDef NVIC_InitStructure;

		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); //时钟使能	 
//	TIM_TimeBaseStructure.TIM_Period = 5000; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	 计数到5000为500ms
//	TIM_TimeBaseStructure.TIM_Prescaler =(7200-1); //设置用来作为TIMx时钟频率除数的预分频值  10Khz的计数频率
		TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	 计数到5000为500ms
		TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值  10Khz的计数频率
		TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
		TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
		TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位

//	TIM_ITConfig(  //使能或者失能指定的TIM中断
//		TIM2, //TIM2
//		TIM_IT_Update  |  //TIM 中断源
//		TIM_IT_Trigger,   //TIM 触发中断源 
//		ENABLE  //使能
//		);

		NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;  //TIM2中断
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;  //先占优先级0级
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //从优先级3级
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
		NVIC_Init(&NVIC_InitStructure);  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器

		TIM_ClearFlag(TIM2,TIM_FLAG_Update);

//	TIM_Cmd(TIM2, ENABLE);  //使能TIMx外设							 
}


void Timer2_enable( void )
{
		TIM_ClearFlag(TIM2, TIM_FLAG_Update); //清除溢出中断标志
		TIM_SetCounter(TIM2,0x00);			//清零计数器值
		TIM_ITConfig(TIM2,TIM_IT_Update|TIM_IT_Trigger,ENABLE);
		TIM_Cmd(TIM2,ENABLE);
}

void Timer2_disable (void)
{
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, DISABLE); //时钟失能
//	TIM_ITConfig(TIM2,TIM_IT_Update,DISABLE);
		TIM_ITConfig(TIM2, TIM_IT_Update | TIM_IT_Trigger,DISABLE );
		TIM_Cmd(TIM2, DISABLE);  //失能TIMx外设
}


void Timer4_enable(u16 arr ,u16 psc)	   	//TIM4使能
{
		TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
		NVIC_InitTypeDef NVIC_InitStructure;

		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); //时钟使能	
		TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	 计数到5000为500ms
		TIM_TimeBaseStructure.TIM_Prescaler = psc; //设置用来作为TIMx时钟频率除数的预分频值  10Khz的计数频率  
		TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
		TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
		TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
 
		TIM_ITConfig(  //使能或者失能指定的TIM中断
		TIM4, //TIM2
		TIM_IT_Update  |  //TIM 中断源
		TIM_IT_Trigger,   //TIM 触发中断源 
		ENABLE  //使能
		);

		NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;  //TIM3中断
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;  //先占优先级0级
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //从优先级3级
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
		NVIC_Init(&NVIC_InitStructure);  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器

		TIM_Cmd(TIM4, ENABLE);  //使能TIMx外设							 
}

void Timer4_disable (void)					   //TIM4失能
{
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, DISABLE); //时钟失能
		TIM_ITConfig(TIM4, TIM_IT_Update | TIM_IT_Trigger,DISABLE );
		TIM_Cmd(TIM4, DISABLE);  //失能TIMx外设
}




#define SYS_TIMx					TIM6
#define SYS_RCC_TIMx			RCC_APB1Periph_TIM6

void Timer_Sys_Init(u32 period_num)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	
	//基础设置，时基和比较输出设置，由于这里只需定时，所以不用OC比较输出
	RCC_APB1PeriphClockCmd(SYS_RCC_TIMx,ENABLE);
	
	TIM_DeInit(SYS_TIMx);

	TIM_TimeBaseStructure.TIM_Period=period_num;//装载值
	//prescaler is 1200,that is 168000000/168/500=2000Hz;
	TIM_TimeBaseStructure.TIM_Prescaler=84-1;//分频系数

	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; //or TIM_CKD_DIV2 or TIM_CKD_DIV4

	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;
	
	TIM_TimeBaseInit(SYS_TIMx,&TIM_TimeBaseStructure);

	TIM_ClearFlag(SYS_TIMx,TIM_FLAG_Update);

	TIM_ITConfig(SYS_TIMx,TIM_IT_Update,ENABLE);

//	TIM_Cmd(SYS_TIMx,ENABLE);
}

void Timer_Sys_Run(u8 ena)
{
	if(ena)
		TIM_Cmd(SYS_TIMx,ENABLE);
	else
		TIM_Cmd(SYS_TIMx,DISABLE);
}



//****************timer3做从机帧定时用用***********************
void TIM3_IRQHandler(void)   //TIM3中断                
{
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET) //检查指定的TIM中断发生与否:TIM 中断源 
	{
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);  //清除TIMx的中断待处理位:TIM 中断源 
		//LED1=!LED1;									   //接收完成闪灯
		Timer3_disable();
		//modbus_com_over=2;
		//checkComm0Modbus();
	}
}


//****************timer2做主机帧定时用***********************
void TIM2_IRQHandler(void)   //TIM2中断
{
		if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET) //检查指定的TIM中断发生与否:TIM 中断源 
		{
				TIM_ClearITPendingBit(TIM2, TIM_IT_Update);  //清除TIMx的中断待处理位:TIM 中断源 
				//LED0=!LED0;									   //接收完成闪灯
				Timer2_disable();
				modbus_com2_over=2;
		}
}

//定时采集数据
void TIM4_IRQHandler(void)   //TIM4中断
{
	if (TIM_GetITStatus(TIM4, TIM_IT_Update) == SET) //检查指定的TIM中断发生与否:TIM 中断源 
	{
		TIM_ClearITPendingBit(TIM4, TIM_IT_Update);  //清除TIMx的中断待处理位:TIM 中断源 
		//modbus_rtu();
	}
}


/***********************************************************************
几个中断的优先级设置

中断源  | 作用     | 抢占级  |   响应级
----------------------------------------
UART1 	| 对后台   |	3	 |	   2
----------------------------------------
UART2 	| 对从机   |	3	 |	   2
----------------------------------------
TIM3 	| 对后台   |	3	 |	   3
----------------------------------------
TIM2 	| 对从机   |	3	 |	   3
----------------------------------------



***********************************************************************/








