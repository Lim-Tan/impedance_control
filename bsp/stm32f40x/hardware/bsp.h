#include "sys.h"


#define OS0_0()  GPIO_ResetBits(GPIOC,GPIO_Pin_0)    //
#define OS0_1()  GPIO_SetBits(GPIOC,GPIO_Pin_0)

#define OS1_0()  GPIO_ResetBits(GPIOC,GPIO_Pin_1)   //
#define OS1_1()  GPIO_SetBits(GPIOC,GPIO_Pin_1)

#define OS2_0()  GPIO_ResetBits(GPIOC,GPIO_Pin_2)   //
#define OS2_1()  GPIO_SetBits(GPIOC,GPIO_Pin_2)

#define AD_range5()     GPIO_ResetBits(GPIOD,GPIO_Pin_3)   //
#define AD_range10()    GPIO_SetBits(GPIOD,GPIO_Pin_3)

#define AD_CONVST_LOW()  GPIO_ResetBits(GPIOD,GPIO_Pin_12) //
#define AD_CONVST_HIGH() GPIO_SetBits(GPIOD,GPIO_Pin_12)

#define AD_RESET_LOW()  GPIO_ResetBits(GPIOC,GPIO_Pin_3)  //
#define AD_RESET_HIGH()  GPIO_SetBits(GPIOC,GPIO_Pin_3)

#define  AD_CS_LOW()    GPIO_ResetBits(GPIOD,GPIO_Pin_6)  //
#define  AD_CS_HIGH()   GPIO_SetBits(GPIOD,GPIO_Pin_6)

#define  AD_CLK_LOW()    GPIO_ResetBits(GPIOD,GPIO_Pin_11) //
#define  AD_CLK_HIGH()   GPIO_SetBits(GPIOD,GPIO_Pin_11)

#define DATA 		GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_0) //PE0


void bsp_InitAD7606(void);

void ad7606_Reset(void);
	
void AD7606_SetOS(u16 ucOS);
	
void ad7606_StartConv(void);

void bsp_thread_entry(void* parameter);

s16  ReceiveData(void);
