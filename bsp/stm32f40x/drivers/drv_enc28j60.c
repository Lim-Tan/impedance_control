/*
 * File      : drv_ssd1351.c
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
 * 2018-01-10     DQL        first implementation.
 */

#include "board.h"          
#include "drv_enc28j60.h"
#include "enc28j60.h"

#define SPI_BUS_NAME                "spi1"
#define SPI_ENC28J60_DEVICE_NAME    "spi10"
#define INT_PIN   17   /* PC2,you can find pin defines in gpio.c */
/*RES_PIN reset signal input. When the pin is low,
initialization of the chip is executed.*/
#define RES_PIN  18  /* PC3,you can find pin defines in gpio.c */
#define CS_PIN   29  /* PA4, ,pin?????drv_gpio.c?pin_index pins[]?????,?????STM32F407ZGT6??,???????144 */

#define OLED_DEBUG

#ifdef  OLED_DEBUG
#define OLED_TRACE         rt_kprintf
#else
#define OLED_TRACE(...)
#endif 

/*
pin defines for 4-wire spi

This pin is Data/Command control pin. When the pin is pulled high,
the input at D7~D0 is treated as display data.When the pin is pulled
low, the input at D7~D0 will be transferred to the command register.

When serial mode is selected, D1 will be the serial data input SDIN
and D0 will be the serial clock input SCLK.

STM32F407ZGT6 SPI1 default GPIOs
    PA5------SPI1SCK
    PA6------SPI1MISO
    PA7------SPI1MOSI

GPIO connections
  STM32F407ZGT6      SSD1351 OLED module
    PA5---SCK----------SCK
    PA7---MOSI---------SI
	PA6---MISO---------SO
    PC3----------------RES
	PC2----------------INT
    PA4----------------CS

*/

struct stm32_hw_spi_cs
{
    rt_uint32_t pin;
};

static struct rt_spi_device spi_dev_enc28j60; /* SPI??ssd1351?? */
static struct stm32_hw_spi_cs  spi_cs;	/* SPI??CS???? */

void hdr_callback(void *args)
{
	enc28j60_isr();
}




static int rt_hw_enc28j60_init(void)
{
    rt_pin_mode(INT_PIN, PIN_MODE_INPUT_PULLUP);

	rt_pin_attach_irq(INT_PIN, PIN_IRQ_MODE_FALLING, hdr_callback, (void*)"callback args");

	rt_pin_irq_enable(INT_PIN, PIN_IRQ_ENABLE); 

	//rt_pin_mode(INT_PIN, PIN_MODE_OUTPUT);
    rt_pin_mode(RES_PIN, PIN_MODE_OUTPUT);

    rt_pin_write(RES_PIN, PIN_LOW);
    //wait at least 100ms for reset
    rt_thread_delay(RT_TICK_PER_SECOND / 10);
    rt_pin_write(RES_PIN, PIN_HIGH);
	
	stm32_spi_bus_attach_device(CS_PIN,SPI_BUS_NAME,SPI_ENC28J60_DEVICE_NAME);
	
	enc28j60_attach(SPI_ENC28J60_DEVICE_NAME);

    return 0;
}
//INIT_PREV_EXPORT(rt_hw_enc28j60_init);	/* ??RT-Thread?????????? */









