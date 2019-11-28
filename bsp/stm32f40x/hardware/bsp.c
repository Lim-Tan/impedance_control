
/*********************************************************************************************************
*	                                  
*	ģ������ : AD7606����ģ��
*	�ļ����� : bsp.c
*	��    �� : V1.1
*	˵    �� : ����AD7606 ADCת���� SPI�ӿ�
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

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);//ʹ��GPIDʱ��

  //GPIOD0~7,E0��ʼ������
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3|GPIO_Pin_6|GPIO_Pin_12|GPIO_Pin_11;//����ӿڵĶ���
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
  GPIO_Init(GPIOD, &GPIO_InitStructure);//��ʼ��GPIO		
	

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);//ʹ��GPICʱ��
	  //GPIOC0~3,E0��ʼ������
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3;//����ӿڵĶ���
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
  GPIO_Init(GPIOC, &GPIO_InitStructure);//��ʼ��GPIO	

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);//ʹ��GPIOEʱ��
 
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0; //MISO��Ӧ����  �������������
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//��ͨ����ģʽ
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100M
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
  GPIO_Init(GPIOE, &GPIO_InitStructure);//��ʼ��GPIOE0
	
	GPIO_SetBits(GPIOD,GPIO_Pin_3);  //���ò�����Χ Ϊ 10v
}

/*
*********************************************************************************************************
*	�� �� ��: ad7606_Reset
*	����˵��: Ӳ����λAD7606
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void ad7606_Reset(void)
{
	/* AD7606�Ǹߵ�ƽ��λ��Ҫ����С����50ns */
	
	AD_RESET_LOW();
	
	AD_RESET_HIGH();
	AD_RESET_HIGH();
	AD_RESET_HIGH();
	AD_RESET_HIGH();
	
	AD_RESET_LOW();
}

/*
*********************************************************************************************************
*	�� �� ��: ad7606_SetOS
*	����˵��: ���ù�����ģʽ�������˲���Ӳ����ƽ��ֵ)
*	��    �Σ�_ucMode : 0-6  0��ʾ�޹�������1��ʾ2����2��ʾ4����3��ʾ8����4��ʾ16��
*				5��ʾ32����6��ʾ64��
*	�� �� ֵ: ��
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
*	�� �� ��: ad7606_StartConv
*	����˵��: ����AD7606��ADCת��
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void ad7606_StartConv(void)
{
	/* �����ؿ�ʼת�����͵�ƽ����ʱ������25ns  */
	AD_CONVST_LOW();
	AD_CONVST_LOW();
	AD_CONVST_LOW();	/* ����ִ��2�Σ��͵�ƽԼ50ns */
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
	for(count=0;count<16;count++)//����16λ����
	{
		AD_CLK_LOW();	//�½�����Ч
		delay_us(1);
		if(DATA)
		Num|=0x01;
		AD_CLK_HIGH();
		Num<<=1;
		
		AD_CLK_HIGH();
	}
	return  Num;
}





