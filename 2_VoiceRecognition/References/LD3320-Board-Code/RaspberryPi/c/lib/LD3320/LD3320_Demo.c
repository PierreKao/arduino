/******************************************************************************
*****************************LD3320 test demo**********************************
* | file      	:	LD3320_Demo.c
* |	version			:	V1.0
* | date				:	2020-06-22
* | function		:	LD3320 test demo	
******************************************************************************/
#include "LD3320_Demo.h"
#include "stdio.h"

extern UBYTE nAsrStatus;	
extern UDOUBLE nMp3Size;
extern UBYTE bMp3Play;	
extern UBYTE nLD_Mode;
extern volatile UBYTE flag_irq;
extern FILE *fp;

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
		if(flag_irq) {
			ProcessInt();
		}
		if(bMp3Play) {
			// printf("*********playing*********\r\n");
			continue;
		}
		// printf("nAsrStatus is %x, nLD_Mode is %x \r\n", nAsrStatus, nLD_Mode);
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
				// printf("Heading Code:%d\r\n", nAsrRes);			 										
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
		Board_text(nAsrRes);//do list
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
		case CODE_RUN:				//Commond "liu shui deng"
			LED_run();
			break;
		case CODE_KEY:				//Commond "an jian"
			LED_key();
			break;
		case CODE_FLASH:			//Commond "shan shuo"
			LED_flash();
			break;
		case CODE_PLAY:				//Commond "bo fang"
			Play_demo("test.mp3");
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

}

/********************************************************************************
function:	
				LED flash
********************************************************************************/
void LED_flash(void)
{

}

/********************************************************************************
function:	
				LED reversal
********************************************************************************/
void LED_key(void)
{

}

/********************************************************************************
function:	
				Play_mp3
********************************************************************************/
void Play_demo(char *path)
{
	if((fp = fopen(path, "r"))  == NULL) {		//open file
		printf("open failed\r\n");
		return;
	}
	
	UBYTE current_position = ftell(fp);			
	fseek(fp, 0, SEEK_END);
	nMp3Size = ftell(fp);						//get file size
	fseek(fp, current_position, SEEK_SET);

	bMp3Play = 1;								//playing status
	LD_Init_MP3();
	LD_Adjust_Volume(1);						//adjust volume
	LD_play();									//start play
}

