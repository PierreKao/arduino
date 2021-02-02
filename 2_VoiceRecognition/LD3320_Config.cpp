/******************************************************************************
**************************Hardware interface layer*****************************
  | file      	:	LD3320_Config.c
  |	version			:	V1.0
  | date				:	2020-06-20
  | function		:	Provide the hardware underlying interface
******************************************************************************/
#include "LD3320_Config.h"

/********************************************************************************
  function: 
        System Init
********************************************************************************/
void System_Init(void)
{
  //set pin
  pinMode(CS, OUTPUT);
  pinMode(RST, OUTPUT);
  pinMode(WR, OUTPUT);
  pinMode(IRQ, INPUT_PULLUP);
  
  //set Serial
  Serial.begin(115200);
  
  //set SPI
  SPI.setDataMode(SPI_MODE2);
  SPI.setBitOrder(MSBFIRST);
  SPI.setClockDivider(SPI_CLOCK_DIV8);
  SPI.begin();
}

/********************************************************************************
  function:
				SPI send data
********************************************************************************/
UBYTE spi_send_byte(UBYTE value)
{
  return SPI.transfer(value);
}

/********************************************************************************
  function:
				Delay
********************************************************************************/
void Driver_Delay_ms(uint32_t xms)
{
  delay(xms);
}
