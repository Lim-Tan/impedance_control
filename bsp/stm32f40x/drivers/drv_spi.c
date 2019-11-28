/*
 * File      : gpio.c
 * This file is part of RT-Thread RTOS
 * COPYRIGHT (C) 2015, RT-Thread Development Team
 *
 * The license and distribution terms for this file may be
 * found in the file LICENSE in this distribution or at
 * http://www.rt-thread.org/license/LICENSE
 *
 * Change Logs:
 * Date           Author            Notes
 * 2017-11-08     ZYH            the first version
 */
#include "board.h"

#include <rtthread.h>
#include <rtdevice.h>
#include <rthw.h>
#include "drv_spi.h"


#ifdef RT_USING_SPI
#define SPIRXEVENT 0x01
#define SPITXEVENT 0x02
#define SPITIMEOUT 2
#define SPICRCEN 0
struct stm32_hw_spi_cs
{
    rt_uint32_t pin;
};

struct stm32_spi
{
    SPI_TypeDef *Instance;
    struct rt_spi_configuration *cfg;
};

//spix=spi1
static rt_err_t stm32_spi_init(SPI_TypeDef *spix, struct rt_spi_configuration *cfg)  
{	 
  GPIO_InitTypeDef  GPIO_InitStructure;
  SPI_InitTypeDef  SPI_InitStructure;
	
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);//那1?邦GPIOA那㊣?車
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);//那1?邦SPI1那㊣?車
 
  //GPIOFB3,4,53?那??‘谷豕??
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;//PB3~5?∩車?1|?邦那?3?	
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//?∩車?1|?邦
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//赤?赤足那?3?
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//谷?角-
  GPIO_Init(GPIOA, &GPIO_InitStructure);//3?那??‘
	
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource5,GPIO_AF_SPI1); //PB3?∩車??a SPI1
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource6,GPIO_AF_SPI1); //PB4?∩車??a SPI1
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource7,GPIO_AF_SPI1); //PB5?∩車??a SPI1
 
	//?a角???????SPI?迆3?那??‘
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_SPI1,ENABLE);//?∩??SPI1
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_SPI1,DISABLE);//赤㏒?1?∩??SPI1

	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //谷豕??SPI米ㄓ?辰?辰?????辰米?那y?Y?㏒那?:SPI谷豕???a???????辰豕???1∟
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;		//谷豕??SPI1∟℅¯?㏒那?:谷豕???a?¯SPI
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;		//谷豕??SPI米?那y?Y∩車D?:SPI﹞⊿?赤?車那?8?????芍11
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;		//∩?DD赤?2?那㊣?車米????D℅∩足??a??米???
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;	//∩?DD赤?2?那㊣?車米?米迆?t??足?㊣???㏒“谷?谷y?辰???米㏒?那y?Y㊣?2谷?迄
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;		//NSSD?o?車谷車2?t㏒“NSS1邦??㏒??1那?豕赤?t㏒“那1車?SSI??㏒?1邦角赤:?迆2?NSSD?o?車DSSI??????
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;		//?“辰?2“足??那?∟﹞??米米??米:2“足??那?∟﹞??米?米?a256
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;	//???“那y?Y∩?那?∩車MSB???1那?LSB???a那?:那y?Y∩?那?∩車MSB???a那?
	SPI_InitStructure.SPI_CRCPolynomial = 7;	//CRC?米????米??角??那?
	SPI_Init(SPI1, &SPI_InitStructure);  //?迄?YSPI_InitStruct?D???“米?2?那y3?那??‘赤a谷豕SPIx??∩??¯
	
	
	SPI_Cmd(SPI1, ENABLE);
	SPI_I2S_ReceiveData(SPI1);
	return RT_EOK;
 	 
}   



#define SPISTEP(datalen) (((datalen) == 8) ? 1 : 2)
#define SPISEND_1(reg, ptr, datalen)       \
    do                                     \
    {                                      \
        if (datalen == 8)                  \
        {                                  \
            (reg) = *(rt_uint8_t *)(ptr);  \
        }                                  \
        else                               \
        {                                  \
            (reg) = *(rt_uint16_t *)(ptr); \
        }                                  \
    } while (0)
#define SPIRECV_1(reg, ptr, datalen)      \
    do                                    \
    {                                     \
        if (datalen == 8)                 \
        {                                 \
            *(rt_uint8_t *)(ptr) = (reg); \
        }                                 \
        else                              \
        {                                 \
            *(rt_uint16_t *)(ptr) = reg;  \
        }                                 \
    } while (0)

static rt_err_t spitxrx1b(struct stm32_spi *hspi, void *rcvb, const void *sndb)
{
    rt_uint32_t padrcv = 0;
    rt_uint32_t padsnd = 0xFF;
    if (!rcvb && !sndb)
    {
        return RT_ERROR;
    }
    if (!rcvb)
    {
        rcvb = &padrcv;
    }
    if (!sndb)
    {
        sndb = &padsnd;
    }
    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET)
        ;
    SPISEND_1(hspi->Instance->DR, sndb, hspi->cfg->data_width);
    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET)
        ;
    SPIRECV_1(hspi->Instance->DR, rcvb, hspi->cfg->data_width);
    return RT_EOK;
}



static rt_uint32_t spixfer(struct rt_spi_device *device, struct rt_spi_message *message)
{
    rt_err_t res;
	struct stm32_spi *hspi;
	struct stm32_hw_spi_cs *cs;
	const rt_uint8_t *sndb;
	rt_int32_t length;
	rt_uint8_t *rcvb;
    RT_ASSERT(device != RT_NULL);
    RT_ASSERT(device->bus != RT_NULL);
    RT_ASSERT(device->bus->parent.user_data != RT_NULL);
    hspi = (struct stm32_spi *)device->bus->parent.user_data;
    cs = device->parent.user_data;
    if (message->cs_take)
    {
        rt_pin_write(cs->pin, 0);
    }
    sndb = message->send_buf;
    rcvb = message->recv_buf;
    length = message->length;
    while (length)
    {
        res = spitxrx1b(hspi, rcvb, sndb);
        if (rcvb)
        {
            rcvb += SPISTEP(hspi->cfg->data_width);
        }
        if (sndb)
        {
            sndb += SPISTEP(hspi->cfg->data_width);
        }
        if (res != RT_EOK)
        {
            break;
        }
        length--;
    }
    /* Wait until Busy flag is reset before disabling SPI */
    while (SPI_I2S_GetFlagStatus(SPI1, SPI_FLAG_BSY) == SET)
        ;
    if (message->cs_release)
    {
        rt_pin_write(cs->pin, 1);
    }
    return message->length - length;
}

rt_err_t spi_configure(struct rt_spi_device *device,
                       struct rt_spi_configuration *configuration)
{
    struct stm32_spi *hspi = (struct stm32_spi *)device->bus->parent.user_data;
    hspi->cfg = configuration;
    return stm32_spi_init(hspi->Instance, configuration);
}
const struct rt_spi_ops stm_spi_ops =
{
    spi_configure,
    spixfer,
};


struct rt_spi_bus _spi_bus1, _spi_bus2, _spi_bus3;
struct stm32_spi _spi1, _spi2, _spi3;
int stm32_spi_register_bus(SPI_TypeDef *SPIx, const char *name)
{
    struct rt_spi_bus *spi_bus;
    struct stm32_spi *spi;
    if (SPIx == SPI1)
    {
        spi_bus = &_spi_bus1;
        spi = &_spi1;
    }
    else if (SPIx == SPI2)
    {
        spi_bus = &_spi_bus2;
        spi = &_spi2;
    }
    else if (SPIx == SPI3)
    {
        spi_bus = &_spi_bus3;
        spi = &_spi3;
    }
    else
    {
        return -1;
    }
    spi->Instance = SPIx;
    spi_bus->parent.user_data = spi;
    return rt_spi_bus_register(spi_bus, name, &stm_spi_ops);
}

//cannot be used before completion init
rt_err_t stm32_spi_bus_attach_device(rt_uint32_t pin, const char *bus_name, const char *device_name)
{
    struct stm32_hw_spi_cs *cs_pin;
	struct rt_spi_device *spi_device = (struct rt_spi_device *)rt_malloc(sizeof(struct rt_spi_device));
    RT_ASSERT(spi_device != RT_NULL);
    cs_pin = (struct stm32_hw_spi_cs *)rt_malloc(sizeof(struct stm32_hw_spi_cs));
    RT_ASSERT(cs_pin != RT_NULL);
    cs_pin->pin = pin;
    rt_pin_mode(pin, PIN_MODE_OUTPUT);
    rt_pin_write(pin, 1);
    return rt_spi_bus_attach_device(spi_device, device_name, bus_name, (void *)cs_pin);
}






#define RT_USING_SPI1
int stm32_hw_spi_init(void)
{
    int result = 0;
#ifdef RT_USING_SPI1
    result = stm32_spi_register_bus(SPI1, "spi1");
#endif
#ifdef RT_USING_SPI2
    result = stm32_spi_register_bus(SPI2, "spi2");
#endif
#ifdef RT_USING_SPI3
    result = stm32_spi_register_bus(SPI3, "spi3");
#endif
    return result;
}
INIT_BOARD_EXPORT(stm32_hw_spi_init);

//void HAL_SPI_MspInit(SPI_HandleTypeDef *spiHandle)
//{
//    GPIO_InitTypeDef GPIO_InitStruct;
//    if (spiHandle->Instance == SPI1)
//    {
//        /* SPI1 clock enable */
//        __HAL_RCC_SPI1_CLK_ENABLE();
//        __HAL_RCC_GPIOA_CLK_ENABLE();
//        /**SPI1 GPIO Configuration
//        PA5     ------> SPI1_SCK
//        PA6     ------> SPI1_MISO
//        PA7     ------> SPI1_MOSI
//        */
//        GPIO_InitStruct.Pin = GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7;
//        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
//        GPIO_InitStruct.Pull = GPIO_NOPULL;
//        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
//        GPIO_InitStruct.Alternate = GPIO_AF5_SPI1;
//        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
//    }
//    else if (spiHandle->Instance == SPI2)
//    {
//        /* SPI2 clock enable */
//        __HAL_RCC_SPI2_CLK_ENABLE();
//        __HAL_RCC_GPIOB_CLK_ENABLE();
//        /**SPI2 GPIO Configuration
//        PB13     ------> SPI2_SCK
//        PB14     ------> SPI2_MISO
//        PB15     ------> SPI2_MOSI
//        */
//        GPIO_InitStruct.Pin = GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15;
//        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
//        GPIO_InitStruct.Pull = GPIO_NOPULL;
//        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
//        GPIO_InitStruct.Alternate = GPIO_AF5_SPI2;
//        HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
//    }
//    else if (spiHandle->Instance == SPI3)
//    {
//        /* SPI3 clock enable */
//        __HAL_RCC_SPI3_CLK_ENABLE();
//        __HAL_RCC_GPIOC_CLK_ENABLE();
//        /**SPI3 GPIO Configuration
//        PC10     ------> SPI3_SCK
//        PC11     ------> SPI3_MISO
//        PC12     ------> SPI3_MOSI
//        */
//        GPIO_InitStruct.Pin = GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12;
//        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
//        GPIO_InitStruct.Pull = GPIO_NOPULL;
//        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
//        GPIO_InitStruct.Alternate = GPIO_AF6_SPI3;
//        HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
//    }
//}

//void HAL_SPI_MspDeInit(SPI_HandleTypeDef *spiHandle)
//{
//    if (spiHandle->Instance == SPI1)
//    {
//        /* Peripheral clock disable */
//        __HAL_RCC_SPI1_CLK_DISABLE();
//        /**SPI1 GPIO Configuration
//        PA5     ------> SPI1_SCK
//        PA6     ------> SPI1_MISO
//        PA7     ------> SPI1_MOSI
//        */
//        HAL_GPIO_DeInit(GPIOA, GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7);
//    }
//    else if (spiHandle->Instance == SPI2)
//    {
//        /* Peripheral clock disable */
//        __HAL_RCC_SPI2_CLK_DISABLE();
//        /**SPI2 GPIO Configuration
//        PB13     ------> SPI2_SCK
//        PB14     ------> SPI2_MISO
//        PB15     ------> SPI2_MOSI
//        */
//        HAL_GPIO_DeInit(GPIOB, GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15);
//    }
//    else if (spiHandle->Instance == SPI3)
//    {
//        /* Peripheral clock disable */
//        __HAL_RCC_SPI3_CLK_DISABLE();
//        /**SPI3 GPIO Configuration
//        PC10     ------> SPI3_SCK
//        PC11     ------> SPI3_MISO
//        PC12     ------> SPI3_MOSI
//        */
//        HAL_GPIO_DeInit(GPIOC, GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12);
//    }
//}
#endif /*RT_USING_SPI*/
