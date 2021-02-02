/******************************************************************************
**************************Hardware interface layer*****************************
* | file      	:	LD3320_Config.c
* |	version			:	V1.0
* | date				:	2020-06-22
* | function		:	Provide the hardware underlying interface	
******************************************************************************/

#include "LD3320_Config.h"
#include <unistd.h>

extern UBYTE nLD_Mode;								// Record the status of LD3320(ASR or Play Sound)
extern UBYTE bMp3Play;								// Record the status of audio player

/******************************************************************************
									GPIO
******************************************************************************/
void DEV_Digital_Write(UWORD Pin, UBYTE Value)
{
#ifdef USE_BCM2835_LIB
    bcm2835_gpio_write(Pin, Value);
    
#elif USE_WIRINGPI_LIB
    digitalWrite(Pin, Value);
    
#elif USE_DEV_LIB
    SYSFS_GPIO_Write(Pin, Value);
    
#endif
}

UBYTE DEV_Digital_Read(UWORD Pin)
{
    UBYTE Read_value = 0;
#ifdef USE_BCM2835_LIB
    Read_value = bcm2835_gpio_lev(Pin);
    
#elif USE_WIRINGPI_LIB
    Read_value = digitalRead(Pin);
    
#elif USE_DEV_LIB
    Read_value = SYSFS_GPIO_Read(Pin);
#endif
    return Read_value;
}

void DEV_GPIO_Mode(UWORD Pin, UWORD Mode)
{
#ifdef USE_BCM2835_LIB  
    if(Mode == 0 || Mode == BCM2835_GPIO_FSEL_INPT){
        bcm2835_gpio_fsel(Pin, BCM2835_GPIO_FSEL_INPT);
		bcm2835_gpio_set_pud(Pin, BCM2835_GPIO_PUD_UP);
    }else {
        bcm2835_gpio_fsel(Pin, BCM2835_GPIO_FSEL_OUTP);
    }
#elif USE_WIRINGPI_LIB
    if(Mode == 0 || Mode == INPUT){
        pinMode(Pin, INPUT);
        pullUpDnControl(Pin, PUD_UP);
    }else{ 
        pinMode(Pin, OUTPUT);
        // printf (" %d OUT \r\n",Pin);
    }
#elif USE_DEV_LIB
    SYSFS_GPIO_Export(Pin);
    if(Mode == 0 || Mode == SYSFS_GPIO_IN){
        SYSFS_GPIO_Direction(Pin, SYSFS_GPIO_IN);
        // printf("IN Pin = %d\r\n",Pin);
    }else{
        SYSFS_GPIO_Direction(Pin, SYSFS_GPIO_OUT);
        // printf("OUT Pin = %d\r\n",Pin);
    }
#endif   
}

void DEV_GPIO_Init(void)
{
    DEV_GPIO_Mode(LD_CS, 1);	
	DEV_GPIO_Mode(LD_WR, 1);
    DEV_GPIO_Mode(LD_RST, 1);
	DEV_GPIO_Mode(LD_IRQ, 0);
}

/******************************************************************************
function:	Module Initialize, the library and initialize the pins, SPI protocol
******************************************************************************/
UBYTE DEV_ModuleInit(void)
{
	
 #ifdef USE_BCM2835_LIB
    if(!bcm2835_init()) {
        printf("bcm2835 init failed  !!! \r\n");
        return 1;
    } else {
        printf("bcm2835 init success !!! \r\n");
    }
	DEV_GPIO_Init();
	printf("USE_SPI\r\n");  
	bcm2835_spi_begin();                                         //Start spi interface, set spi pin for the reuse function
	bcm2835_spi_setBitOrder(BCM2835_SPI_BIT_ORDER_MSBFIRST);     //High first transmission
	bcm2835_spi_setDataMode(BCM2835_SPI_MODE2);                  //spi mode 2
	bcm2835_spi_setClockDivider(BCM2835_SPI_CLOCK_DIVIDER_512);  //Frequency  speed = 500MHz / 512
	bcm2835_spi_chipSelect(BCM2835_SPI_CS0);                     //set CE0
	bcm2835_spi_setChipSelectPolarity(BCM2835_SPI_CS0, LOW);     //enable cs0
    
#elif USE_WIRINGPI_LIB  
    //if(wiringPiSetup() < 0) {//use wiringpi Pin number table  
    if(wiringPiSetupGpio() < 0) { //use BCM2835 Pin number table
        printf("set wiringPi lib failed	!!! \r\n");
        return 1;
    } else {
        printf("set wiringPi lib success  !!! \r\n");
    }
	DEV_GPIO_Init();
	printf("USE_SPI\r\n");
	wiringPiSPISetupMode(0, 1000000, 2);
   
#elif USE_DEV_LIB
	DEV_GPIO_Init();
	printf("USE_SPI\r\n"); 
	DEV_HARDWARE_SPI_begin("/dev/spidev0.0");
#endif
    return 0;
}

/********************************************************************************
function:	
				SPI send data
********************************************************************************/
UBYTE spi_send_byte(UBYTE Value)
{
#ifdef USE_BCM2835_LIB
    return bcm2835_spi_transfer(Value);
    
#elif USE_WIRINGPI_LIB
	wiringPiSPIDataRW(0,&Value,1);
    return Value;
    
#elif USE_DEV_LIB
    return DEV_HARDWARE_SPI_TransferByte(Value); 
#endif
	
}

/********************************************************************************
function:		
				Delay 
********************************************************************************/
void Driver_Delay_ms(uint32_t xms)
{
#ifdef USE_BCM2835_LIB
    bcm2835_delay(xms);
#elif USE_WIRINGPI_LIB
    delay(xms);
#elif USE_DEV_LIB
    UDOUBLE i;
    for(i=0; i < xms; i++){
        usleep(1000);
    }
#endif
}

/******************************************************************************
function:	Module exits, closes SPI and BCM2835 library
parameter:
Info:
******************************************************************************/
void DEV_ModuleExit(void)
{
#ifdef USE_BCM2835_LIB
    bcm2835_spi_end();
    bcm2835_close();

#elif USE_WIRINGPI_LIB
    CS_0;
	RST_1;
	WR_0;

#elif USE_DEV_LIB
    CS_0;
	RST_1;
	WR_0;
    DEV_HARDWARE_SPI_end();
#endif
}
