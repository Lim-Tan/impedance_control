//#include <core_cm4.h>
#include "delay.h"
#include <rtthread.h>
//#include "stm32f4xx.h"


void rt_hw_us_delay(int us)
{
rt_uint32_t delta;

us = us * (SysTick->LOAD/(1000000/RT_TICK_PER_SECOND));

delta = SysTick->VAL;

while (delta - SysTick->VAL< us);
}

void delay_us(u16 time)
{    
   u16 i=0;  
   while(time--)
   {
      i=10;  
      while(i--) ;    
   }
}

void delay_ms(u16 time)
{    
   u16 i=0;  
   while(time--)
   {
      i=12000;  
      while(i--) ;    
   }
}
