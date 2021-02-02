/******************************************************************************
*****************************LD3320 test demo**********************************
* | file      	:	LD3320_Demo.c
* |	version			:	V1.0
* | date				:	2020-06-18
* | function		:	LD3320 test demo	
******************************************************************************/
#include "LD3320_Demo.h"

extern UBYTE 		nAsrStatus;	
extern UDOUBLE 	nMp3Size;
extern UBYTE 		bMp3Play;	
extern UBYTE 		nLD_Mode;

/********************************************************************************
function:	
				LD3320 test demo main function
********************************************************************************/
void LD3320_test(void)
{
	UBYTE nAsrRes = 0;
	LD_init();	   		
	while(1)
	{
		if(bMp3Play) {
			//printf("*********playing*********\r\n");
			continue;
		}
		//printf("nAsrStatus is %x, nLD_Mode is %x \r\n", nAsrStatus, nLD_Mode);
		switch(nAsrStatus) {
			case LD_ASR_RUNING:
			case LD_ASR_ERROR:		
				break;
			case LD_ASR_NONE:
				nAsrStatus=LD_ASR_RUNING;
				if (LD_ASR()==0)//Start the ASR process once	
					nAsrStatus = LD_ASR_ERROR;
				break;
			case LD_ASR_FOUNDOK:
				nAsrRes = LD_GetResult();//once ASR process end, get the result									 	 										
				switch(nAsrRes) {//show the commond
					case CODE_RUN:					
						printf("waterfall led\r\n"); 
						break;
					case CODE_KEY:	 			
						printf("reversal led\r\n"); 
						break;
					case CODE_FLASH:			
						printf("flash led\r\n"); 
						break;
					case CODE_PLAY:			
						printf("play mp3\r\n"); 
						break;							
					default:
						break;
				}	
				nAsrStatus = LD_ASR_NONE;
				break;
			case LD_ASR_FOUNDZERO:
			default:
				nAsrStatus = LD_ASR_NONE;
				break;
		}
		Board_text(nAsrRes);//do the commond
		nAsrRes=0;
	}
}

/********************************************************************************
function:	
				Do the corresponding operation on the board
********************************************************************************/
void Board_text(UBYTE Code_Val)
{																					 
	switch(Code_Val) {
		case CODE_RUN:  	//Commond "liu shui deng"
			LED_run();
			break;
		case CODE_KEY:	  //Commond "an jian"
			LED_key();
			break;
		case CODE_FLASH:	//Commond "shan shuo"
			LED_flash();
			break;
		case CODE_PLAY:		//Commond "bo fang"
			Play_demo();	
			break;
		default:break;
	}
}

/********************************************************************************
function:	
				LED waterfall 
********************************************************************************/
void LED_run(void)
{
	UBYTE i;
	for(i=0; i<3; i++) {
		LED2_1;
		Driver_Delay_ms(100);
		LED3_1;
		Driver_Delay_ms(100);
		LED4_1;
		Driver_Delay_ms(100);
		LED2_0;
		Driver_Delay_ms(100);
		LED3_0;
		Driver_Delay_ms(100);
		LED4_0;
		Driver_Delay_ms(100);
	}
}

/********************************************************************************
function:	
				LED flash
********************************************************************************/
void LED_flash(void)
{
	LED2_0;
	Driver_Delay_ms(300);
	LED2_1;
	Driver_Delay_ms(300);
	LED2_0;
	Driver_Delay_ms(300);
	LED2_1;
	Driver_Delay_ms(300);
	LED2_0;
	Driver_Delay_ms(300);
	LED2_1;
	Driver_Delay_ms(300);
}

/********************************************************************************
function:	
				LED reversal
********************************************************************************/
void LED_key(void)
{
	if(ReadLED2)
		LED2_0;
	else
		LED2_1;
}

/********************************************************************************
function:	
				Play_mp3
********************************************************************************/
void Play_demo()
{
	bMp3Play = 1;
	nMp3Size = size_sound_1 - 1;
	LD_Init_MP3();
	LD_Adjust_Volume(1);
	LD_play(sound_1);
}

