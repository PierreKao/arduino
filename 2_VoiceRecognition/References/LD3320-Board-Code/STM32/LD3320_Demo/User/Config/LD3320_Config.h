/******************************************************************************
**************************Hardware interface layer*****************************
* | file      	:	LD3320_Config.h
* |	version			:	V1.0
* | date				:	2020-06-18
* | function		:	Provide the hardware underlying interface	
******************************************************************************/
#ifndef __LD3320_CONFIG_H
#define __LD3320_CONFIG_H

#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal_gpio.h"
#include "main.h"
#include <stdint.h>
#include <stdio.h>

//data
#define UBYTE   uint8_t
#define UWORD   uint16_t
#define UDOUBLE uint32_t

void Driver_Delay_ms(uint32_t xms);
UBYTE spi_send_byte(UBYTE value);
						
#endif
