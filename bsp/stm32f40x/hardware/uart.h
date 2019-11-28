#ifndef __USART_H
#define __USART_H
//#include "stdio.h"
#include "stm32f4xx.h"

//支持适应不同频率下的串口波特率设置.
//加入了对printf的支持
//增加了串口接收命令功能.
//修正了printf第一个字符丢失的bug


#define receEnable() PAout(7)=0
#define sendEnable() PAout(7)=1

#define receEnable2() PAout(5)=0
#define sendEnable2() PAout(5)=1

void uart1_init(u32 bound);
void uart2_init(u32 bound);
void beginSend(void);
void beginSend2(void);

#endif
