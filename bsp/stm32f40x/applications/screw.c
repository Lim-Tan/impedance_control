/*
 * File      : screw.c
 * This file is part of Project-screw
 *
 *
 * Change Logs:
 * Date           Author       Notes
 * 2019-01-02     sharp      the first version 
 */
 
 /*
     date:2019-2-22  
		 add step_motor_up&down function
		 test the SET in the short term
		 可以在安装光栅的位置设置限定位
 */
 
#include <rtthread.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <dfs_posix.h>

#include "led.h"
#include "key.h"
#include "encode.h"
#include "pid.h"
#include "can.h"
#include "delay.h"
#include "step_time.h"
#include "drv_lcd.h" 
#include "bsp.h"

/**************************************************************************
*                            全局变量                                     *
***************************************************************************/
/* 消息队列控制块 */
static struct rt_messagequeue mq;
static char msg_pool[1024];
/* 信号量控制块 */
static struct rt_semaphore sem;
static struct rt_semaphore sem1;
static char file_name[20];
//static  char *file_name;
rt_mutex_t MUTEX;

/* 字符串Buf */
char BUFFER[24];

#define COUNT_NUM 1
#define AD_OFFSET -460     // 2.txt

#define Base_value 50
#define Cycle_value 12

rt_uint16_t count=0;
rt_uint16_t count1=0;
rt_int16_t count_1=0;
rt_int16_t clock_position=0;
rt_uint16_t mode=4;

rt_uint16_t count2 = 0;

rt_int16_t Desire_value = 0;
rt_int16_t flag = 0;

rt_int16_t v;
rt_int16_t v1;

typedef struct{
    u16 head;
    u16 tick;
    s16 deformation;
   // s16 current;
    s16 ad_torque;   
	
}Comdate;

/**************************************************************************
*                            创建交互函数                                 *
***************************************************************************/
void measure(int argc, char **argv)
{	
	 if (argc < 2)
    {
        rt_kprintf("Usage: measure file_name\n");
        rt_kprintf("Like: measure 1.txt\n");
        return ;
    }
		rt_memset(file_name,sizeof(file_name),0);	
		rt_strncpy(file_name,argv[1],sizeof(file_name));
//	 file_name = argv[1];
  	 //rt_kprintf(" %s \r\n",file_name);
    mode = 0;
    rt_sem_release(&sem);
    rt_sem_release(&sem1);
	
}
MSH_CMD_EXPORT(measure, measure); 

void up_down(int argc, char **argv)
{	
	 if (argc < 2)
    {
        rt_kprintf("Usage: up_down up or down\n");
        rt_kprintf("Like: up_down up \n");
        return ;
    }
    if(!strcmp(argv[1],"up"))
		{
		   mode = 2 ;
		}
		else if(!strcmp(argv[1],"down"))
		{
		   mode = 1 ;
		}
		else if(!strcmp(argv[1],"stop"))
		{
		   Pulse_output(0,0);   // 有问题   会出现不稳定 步进电机 会有微小的来回振动
		}	
}
MSH_CMD_EXPORT(up_down, up_down); 

/**************************************************************************
*                            线程入口                                     *
***************************************************************************/

void pid_thread_entry(void* parameter)
{
	 while(1)
	 {   
		   
			 count = TIM4->CNT;
			 count1 = TIM8->CNT;
//		 	 count = GetEncoder1();
//			 count1 = GetEncoder2();
		   rt_enter_critical();
			 count_1 = count-count1;
//			 count_1 = (count_1+4096)%4096;
		   rt_exit_critical();
//		   rt_kprintf("%d--",count_1);
//		   rt_kprintf("1");
			 ResultValue =SeqIntPID(7,0,28.5,Desire_value,count_1);		
			 if(ResultValue>1800)
					 ResultValue=1800;
			 else if (ResultValue<-1800)
					 ResultValue=-1800; 			
			 CAN_RoboModule_DRV_Current_Velocity_Mode(0,1,6000,ResultValue);
			 rt_thread_delay(10);
	}
}

/* print_test 线程 
   测试橡胶
*/
void print_test_thread(void *parameter)
{
//	  while(1)
//		{
//		if (count_1 > 2048)
//			count_1 = 4096 - count_1;
//    rt_kprintf("%d\n",count_1);   //只打印一次------加循环
//	  rt_thread_delay(20);
//		}
}
	
/* LCD 显示 线程 */
void lcd_thread_entry(void* parameter)
{
		POINT_COLOR=RED; 
	  LCD_ShowString(30,90,200,16,16,"rubber test"); 
	  POINT_COLOR=BLUE;//设置字体为蓝色
	  LCD_ShowString(30,150,200,16,16,"file:");  
	  LCD_ShowString(30,170,200,16,16,"count:");		
	while(1)
	{
		LCD_ShowString(94,150,200,16,16,file_name);    	
		LCD_ShowxNum(94,170,count2,4,16,0);
		rt_thread_delay(20);
	}
}

void key_thread_entry(void* parameter)
{
		rt_uint8_t key;
    while(1)
    {
			key=KEY_Scan(0);
			
			if(key == KEY0_PRES)
			{
				mode=1;
			}	
			
			else if(key == KEY1_PRES)
			{	
				rt_kprintf("%d \r\n",4);
				mode=2;
			}		
			
			else if(key == WKUP_PRES)
			{	
				mode=0;
			}	
			
			else
			{
			  //mode = 4;            //跟measure 中 mode = 0 冲突  不设置为 4 mode 值不变
			}
			
			rt_thread_delay(10);
   }
}
/*  
  360    380    360    480     360    580
  140    320    180    420     180    520
*/
float angle;
void stepmotor_thread_entry(void* parameter)
{
	    while(1)
	    {							
		if(mode == 1)
		{		 
			 LED0_DIR=0;
			 Pulse_output(1000,800);//1KHZ,8000???
		}
		else if(mode == 2)	
		{
			 LED0_DIR=1;
			 Pulse_output(1000,800);
		}
		else if(mode == 0)	
		{
			while(count2 < COUNT_NUM && mode != 2)
			{
				//rt_kprintf("enter 123\r\n");
//        Desire_value = 100;			
//				rt_kprintf("enter 123");
				LED0_DIR=0;
				Pulse_output(265,8500);//3.3KHZ,12000    265  250
//			rt_kprintf("2");
				while(count_1 < 80 || (count_1 > 2048&&count_1<4096))  //200 400
				{
				angle =  OscilElement((float)((200-count_1)*1.8)/1000);	
				rt_kprintf("enter angle = %d--",(int)angle);
				Desire_value = (int)(angle*57.6/0.09);
				rt_kprintf("enter Desire_value = %d--",Desire_value);
				rt_thread_delay(1);  
				}
				Desire_value = 0;
				rt_thread_delay(300);         //延时300ms 是为了让PID程序达到稳定
        Desire_value = 300;
//				rt_kprintf("4");
				while(count_1 < 150 || (count_1 > 2048&&count_1<4096))    //400  500  600
				rt_thread_delay(1);
				Desire_value = 0;
				rt_thread_delay(2000);				// 时间延长
				LED0_DIR=1;
				Desire_value = -650;
				Pulse_output(240,8500);//3.3KHZ,12000    220     有可能没有执行PID，直接运行这句，步进电机先上
				rt_thread_delay(200);
				Desire_value = 0;
				count2++;
				rt_thread_delay(2000);
			} 
			mode = 4;
			Desire_value = 0;
      count2 = 0;
		}			
	  rt_thread_delay(10);
	}
}

/*
  Test rubber linearity thread 
  2019/3/26
  author: Kris_tan
  question: the time that change the Desire_value(200ms) is not equal to read data(10ms)
*/
//void Test_rubber_line_entry(void* parameter)
//{
//	int i = 0;
//	while(1)
//	{
//		if(mode == 0 && count2 < COUNT_NUM)
//		{
//     for(i=0;i<6;i++)
//		 {
//		     Desire_value += 50;
//			   //rt_kprintf("put one");
//			   rt_thread_delay(200);
//		 }
//		 for(i=6;i>-6;i--)
//			 {
//			    Desire_value -= 50;
//				  rt_thread_delay(200);
//			 }
////		 for(i= -12;i<0;i++)
////			 {
////			   Desire_value += 50;
////			   rt_thread_delay(200);
////			 }
//		 ++count2;
//		 rt_thread_delay(10);
//	 }
//		else 
//			Desire_value = 0;
//		  rt_thread_delay(10);
//	}
//	   rt_thread_delay(10);
//}
	
void display_thread_entry(void* parameter)
{
    Comdate comdate;
	  int fd;
	
	while(1){		
    rt_sem_take(&sem1, RT_WAITING_FOREVER);
	  if((fd=open(file_name,O_WRONLY | O_CREAT))==-1)
    printf("open %s error\r\n",file_name);	      
	  
    while (count2 < COUNT_NUM)
    {     			
        if (rt_mq_recv(&mq, &comdate, sizeof(comdate), RT_WAITING_FOREVER)
                == RT_EOK)
        {    
          //rt_kprintf("content count2 is ok\n");					
          write(fd,&comdate,sizeof(comdate));  
					//rt_kprintf("%s",ch);
        }
        rt_thread_delay(1);
    }
		close(fd);
		rt_thread_delay(10);
		printf("close %s \r\n",file_name);	
	}
//    rt_kprintf("%d,%d\n",count_1,v);
}
	
void collect_thread_entry(void* parameter)
{
    Comdate comdate;
    int result;
	while(1)
		{
    rt_sem_take(&sem, RT_WAITING_FOREVER);   //判断有没有 获得信号量
 			
    comdate.head=0x6666;
    while (count2 < COUNT_NUM)
    {
			      //rt_kprintf("count_1 = %d\r\n",count_1);
        if(Desire_value >= 0 && LED0_DIR==0) {             // 判断方向  滤掉上升停止的 数据
	        	ad7606_StartConv();
		        delay_us(2);		
		        v = ReceiveData();             // 第一通道的数据
					  delay_us(2);
		        v1 = ReceiveData();             // 第二通道的数据
          //rt_kprintf(" %d\r\n",count_1);
					
            comdate.tick = rt_tick_get();
            comdate.deformation = count_1;        
           // comdate.current = Real_Current_Value[0];
            comdate.ad_torque = v1;  
            result = rt_mq_send(&mq, &comdate, sizeof(comdate));
            if ( result == -RT_EFULL) {               
                rt_kprintf("message queue full, delay 1s\n");
                rt_thread_delay(1000);
            }
        }
		rt_thread_delay(10);           
    }  
		result = rt_mq_send(&mq, "--the end--\r\n", 13);
		} 
}


/**************************************************************************
*                            创建线程                                     *
***************************************************************************/

int key_thread_init()
{
	rt_thread_t tid;

    tid = rt_thread_create("key",
        key_thread_entry, RT_NULL,
        2048, 5, 20);

    if (tid != RT_NULL)
        rt_thread_startup(tid);
	
    return 0;
}
INIT_APP_EXPORT(key_thread_init);

int lcd_thread_init()
{
	rt_thread_t tid;

    tid = rt_thread_create("lcd",
        lcd_thread_entry, RT_NULL,
        2048, 9, 20);

    if (tid != RT_NULL)
        rt_thread_startup(tid);
	
    return 0;
}
INIT_APP_EXPORT(lcd_thread_init);

int collect_thread_init()
{
	rt_thread_t tid;

    tid = rt_thread_create("collect",
        collect_thread_entry, RT_NULL,
        2048, 6, 20);

    if (tid != RT_NULL)
        rt_thread_startup(tid);
	
    return 0;
}
INIT_APP_EXPORT(collect_thread_init);
	
int pid_thread_init()
{
	rt_thread_t tid;

    tid = rt_thread_create("pid",
        pid_thread_entry, RT_NULL,
        2048, 7, 20);

    if (tid != RT_NULL)
        rt_thread_startup(tid);
    return 0;
}
INIT_APP_EXPORT(pid_thread_init);


int stepmotor_thread_init()
{
	rt_thread_t tid;

    tid = rt_thread_create("stepmotor",
        stepmotor_thread_entry, RT_NULL,
        2048, 8, 20);

    if (tid != RT_NULL)
        rt_thread_startup(tid);	
    return 0;
}
INIT_APP_EXPORT(stepmotor_thread_init);

//int Test_rubber_thread_init()
//{
//    rt_thread_t tid;

//    tid = rt_thread_create("Test_rubber_line",
//        Test_rubber_line_entry, RT_NULL,
//        2048, 15, 20);

//    if (tid != RT_NULL)
//        rt_thread_startup(tid);
//	
//    return 0;
//}
//INIT_APP_EXPORT(Test_rubber_thread_init);	

int display_thread_init()
{
	rt_thread_t tid;

    tid = rt_thread_create("display",
        display_thread_entry, RT_NULL,
        2048, 15, 20);

    if (tid != RT_NULL)
        rt_thread_startup(tid);
	
    return 0;
}
INIT_APP_EXPORT(display_thread_init);

int print_test()
{
	rt_thread_t tid;

    tid = rt_thread_create("print_test",
        print_test_thread, RT_NULL,
        2048, 15, 20);

    if (tid != RT_NULL)
        rt_thread_startup(tid);
	
    return 0;
}
INIT_APP_EXPORT(print_test);

/**************************************************************************
*                            创建IPC对象                                  *
***************************************************************************/

int messageq_simple_init()
{
    /* 初始化消息队列 */
    rt_mq_init(&mq, "mqt", 
    &msg_pool[0], /* 内存池指向msg_pool */
     13 , /* 每个消息的大小是 128 - void*  13 - sizeof(void*)*/
    sizeof(msg_pool),  /* 内存池的大小是msg_pool的大小 */
    RT_IPC_FLAG_FIFO); /* 如果有多个线程等待，按照FIFO的方法分配消息 */
    

        /* 初始化信号量，初始值是0 */       // 用到信号量的原因是要等待 上层输入命令才 开始收集数据
    rt_sem_init(&sem, "sem", 0, 
               RT_IPC_FLAG_FIFO);    //消息队列的方式等待  还有优先级等待方式
		
		rt_sem_init(&sem1, "sem1", 0, 
               RT_IPC_FLAG_FIFO);   
 
		   MUTEX = rt_mutex_create("dmutex", RT_IPC_FLAG_FIFO);
    if (MUTEX == RT_NULL)
    {
        rt_kprintf("create dynamic mutex failed.\n");
        return -1;
    }
		
    return 0;
}
INIT_ENV_EXPORT(messageq_simple_init);

/**************************************************************************
*                            自定义硬件初始化                             *
***************************************************************************/
int hardware_init()
{
	 rt_hw_led_init();
	
	 rt_hw_key_init();
	
	 encode_Init();

	 bsp_InitAD7606();
	 ad7606_Reset();
	 AD7606_SetOS(0);
   GPIO_SetBits(GPIOD,GPIO_Pin_3);
	
	 CAN1_Configuration(); 

	 
	 delay_ms(500);                                      //发送复位指令后的延时必须要有，等待驱动器再次初始化完成
	 CAN_RoboModule_DRV_Reset(0,1);
	 delay_ms(500); 
   CAN_RoboModule_DRV_Config(0,1,9,0);               //1号驱动器配置为9ms传回一次电流数据
   delay_us(200);                                      //此处延时为了不让传回数据时候4个不一起传
	 CAN_RoboModule_DRV_Mode_Choice(0,1,Current_Velocity_Mode);  //0组的所有驱动器 都进入开环模式
   delay_ms(500);                                      //发送模式选择指令后，要等待驱动器进入模式就绪。所以延时也不可以去掉。

	 return 0;
}
INIT_DEVICE_EXPORT(hardware_init);



	
	

	 

