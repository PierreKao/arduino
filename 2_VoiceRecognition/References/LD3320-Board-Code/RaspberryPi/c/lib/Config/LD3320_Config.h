/******************************************************************************
**************************Hardware interface layer*****************************
* | file      	:	LD3320_Config.h
* |	version			:	V1.0
* | date				:	2020-06-22
* | function		:	Provide the hardware underlying interface	
******************************************************************************/
#ifndef __LD3320_CONFIG_H
#define __LD3320_CONFIG_H

#ifdef USE_BCM2835_LIB
    #include <bcm2835.h>
#elif USE_WIRINGPI_LIB
    #include <wiringPi.h>
    #include <wiringPiSPI.h>
#elif USE_DEV_LIB
    #include "RPI_sysfs_gpio.h"
    #include "dev_hardware_SPI.h" 
#endif

#include <unistd.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//Data
#define UBYTE		uint8_t
#define UWORD		uint16_t
#define UDOUBLE		uint32_t

//LD	Define
#define LD_CS			12	
#define LD_WR			27	
#define LD_RST			25	
#define LD_IRQ			22

//LD	GPIO
#define CS_0	DEV_Digital_Write(LD_CS, 0)
#define CS_1	DEV_Digital_Write(LD_CS, 1)

#define WR_0	DEV_Digital_Write(LD_WR, 0)
#define WR_1	DEV_Digital_Write(LD_WR, 1)

#define RST_0	DEV_Digital_Write(LD_RST, 0)
#define RST_1	DEV_Digital_Write(LD_RST, 1)

UBYTE spi_send_byte(UBYTE Value);
UBYTE DEV_Digital_Read(UWORD Pin);
void DEV_Digital_Write(UWORD Pin, UBYTE Value);
void DEV_GPIO_Mode(UWORD Pin, UWORD Mode);
void DEV_GPIO_Init(void);
void DEV_ModuleExit(void);
UBYTE DEV_ModuleInit(void);
void Driver_Delay_ms(uint32_t xms);

#endif
