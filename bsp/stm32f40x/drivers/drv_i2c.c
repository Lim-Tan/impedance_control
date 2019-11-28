/*
 * File      : drv_i2c.c
 * This file is part of RT-Thread RTOS
 * COPYRIGHT (C) 2006 - 2018, RT-Thread Development Team
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 * Change Logs:
 * Date           Author       Notes
 * 2017-06-05     tanek        first implementation.
 * 2018-03-08     ZYH          Porting for stm32f4xx
 */
#include <rthw.h>
#include <rtthread.h>
#include <rtdevice.h>
#include "drv_i2c.h"
#include <board.h>
/*user can change this*/
#define I2C_BUS_NAME  "i2c2"
/*user should change this to adapt specific board*/
#define I2C_SCL_PIN                 GPIO_Pin_8
#define I2C_SCL_PORT                GPIOB
#define I2C_SDA_PIN                 GPIO_Pin_9
#define I2C_SDA_PORT                GPIOB




static void drv_i2c_gpio_init()
{			
  GPIO_InitTypeDef  GPIO_InitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);//使能GPIOB时钟

  //GPIOB8,B9初始化设置
  GPIO_InitStructure.GPIO_Pin = I2C_SCL_PIN | I2C_SDA_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
  GPIO_Init(I2C_SDA_PORT, &GPIO_InitStructure);//初始化
  GPIO_SetBits(I2C_SDA_PORT,I2C_SCL_PIN | I2C_SDA_PIN);
}

static void drv_set_sda(void *data, rt_int32_t state)
{
	GPIO_WriteBit(I2C_SDA_PORT, I2C_SDA_PIN, state ? Bit_SET : Bit_RESET);
}

static void drv_set_scl(void *data, rt_int32_t state)
{
	GPIO_WriteBit(I2C_SCL_PORT, I2C_SCL_PIN, state ? Bit_SET : Bit_RESET);
}


static rt_int32_t  drv_get_sda(void *data)
{
    return GPIO_ReadOutputDataBit(I2C_SDA_PORT, I2C_SDA_PIN) ? 1 : 0;
}

static rt_int32_t  drv_get_scl(void *data)
{
    return GPIO_ReadOutputDataBit(I2C_SCL_PORT, I2C_SCL_PIN) ? 1 : 0;
}

static void drv_udelay(rt_uint32_t time)
{    
   rt_uint32_t i=0;  
   while(time--)
   {
      i=10;  
      while(i--) ;    
   }
}

static const struct rt_i2c_bit_ops drv_bit_ops =
{
    RT_NULL,
    drv_set_sda,
    drv_set_scl,
    drv_get_sda,
    drv_get_scl,
    drv_udelay,
    1,
    100
};

int drv_i2c_init(void)
{
    static struct rt_i2c_bus_device i2c2_bus;
    drv_i2c_gpio_init();
    rt_memset((void *)&i2c2_bus, 0, sizeof(struct rt_i2c_bus_device));
    i2c2_bus.priv = (void *)&drv_bit_ops;
    rt_i2c_bit_add_bus(&i2c2_bus, I2C_BUS_NAME);
    return RT_EOK;
}
INIT_DEVICE_EXPORT(drv_i2c_init);
/* end of i2c driver */
