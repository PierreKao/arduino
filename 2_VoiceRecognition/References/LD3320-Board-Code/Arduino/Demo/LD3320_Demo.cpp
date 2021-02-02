/******************************************************************************
*****************************LD3320 test demo**********************************
* | file      	:	LD3320_Demo.c
* |	version			:	V1.0
* | date				:	2020-06-20
* | function		:	LD3320 test demo	
******************************************************************************/
#include "LD3320_Demo.h"

extern UBYTE 		nAsrStatus;	
extern UDOUBLE 	nMp3Size;
extern UBYTE 		bMp3Play;	

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
      delay(1);
			continue;
		}
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
						Serial.print("waterfall led\r\n"); 
						break;
					case CODE_KEY:	 			
						Serial.print("reversal led\r\n"); 
						break;
					case CODE_FLASH:			
						Serial.print("flash led\r\n"); 
						break;
					case CODE_PLAY:			
						Serial.print("play mp3\r\n"); 
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
    case CODE_RUN:    //Commond "liu shui deng"
      LED_run();
      break;
    case CODE_KEY:    //Commond "an jian"
      LED_key();
      break;
    case CODE_FLASH:  //Commond "shan shuo"
      LED_flash();
      break;
    case CODE_PLAY:   //Commond "bo fang"
      Play_demo();  
      break;
    default:
      break;
  }
}

/********************************************************************************
function:  
        LED waterfall 
********************************************************************************/
void LED_run(void)
{
  Serial.print("no led\r\n"); 
  Serial.print("You can do other things\r\n"); 
}

/********************************************************************************
function: 
        LED flash
********************************************************************************/
void LED_flash(void)
{
  Serial.print("no led\r\n");
  Serial.print("You can do other things\r\n");  
}

/********************************************************************************
function: 
        LED reversal
********************************************************************************/
void LED_key(void)
{
  Serial.print("no led\r\n");
  Serial.print("You can do other things\r\n");  
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
