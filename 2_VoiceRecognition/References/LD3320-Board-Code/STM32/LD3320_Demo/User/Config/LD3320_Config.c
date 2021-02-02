/******************************************************************************
**************************Hardware interface layer*****************************
* | file      	:	LD3320_Config.c
* |	version			:	V1.0
* | date				:	2020-06-18
* | function		:	Provide the hardware underlying interface	
******************************************************************************/
#include "LD3320_Config.h"
#include "spi.h"

/********************************************************************************
function:	
				SPI send data
********************************************************************************/
UBYTE spi_send_byte(UBYTE value)
{
    __HAL_SPI_ENABLE(&hspi1);
    SPI1->CR2 |= (1) << 12;

    while((SPI1->SR & (1 << 1)) == 0)
        ;

    *((__IO uint8_t *)(&SPI1->DR)) = value;

    while(SPI1->SR & (1 << 7)) ; //Wait for not busy

    while((SPI1->SR & (1 << 0)) == 0) ; // Wait for the receiving area to be empty

    return *((__IO uint8_t *)(&SPI1->DR));
}

/********************************************************************************
function:		
				Delay 
********************************************************************************/
void Driver_Delay_ms(uint32_t xms)
{
    HAL_Delay(xms);
}

