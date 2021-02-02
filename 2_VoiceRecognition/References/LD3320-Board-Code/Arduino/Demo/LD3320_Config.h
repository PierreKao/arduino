/******************************************************************************
**************************Hardware interface layer*****************************
* | file      	:	LD3320_Config.h
* |	version			:	V1.0
* | date				:	2020-06-20
* | function		:	Provide the hardware underlying interface	
******************************************************************************/
#ifndef __LD3320_CONFIG_H
#define __LD3320_CONFIG_H

#include <stdio.h>    
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <SPI.h>
#include <Wire.h>

//data
#define UBYTE   uint8_t
#define UWORD   uint16_t
#define UDOUBLE uint32_t

//LD	GPIO
#define CS        10
#define WR        8
#define RST       9
#define IRQ       2

void System_Init(void);
void Driver_Delay_ms(uint32_t xms);
UBYTE spi_send_byte(UBYTE value);				
		
#endif
