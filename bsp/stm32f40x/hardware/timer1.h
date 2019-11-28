#ifndef __TIMER_H
#define __TIMER_H
#include "stm32f4xx.h"

void Timer3_Init(u16 arr,u16 psc);
void Timer3_enable(void);
void Timer3_disable(void);

void Timer2_Init(u16 arr ,u16 psc);
void Timer2_enable(void);
void Timer2_disable(void);

void Timer4_enable(u16 arr,u16 psc);
void Timer4_disable(void);

void Timer_Sys_Init(u32 period_num);
void Timer_Sys_Run(u8 ena);

//extern u8 modbus_com_over;
//extern u8 modbus_com2_over;

#endif
