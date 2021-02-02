/******************************************************************************
*******************************Driver for LD3320*******************************
* | file      	:	LD3320_Driver.c
* |	version			:	V1.0
* | date				:	2020-06-20
* | function		:	Driver for LD3320
******************************************************************************/
#include "LD3320_Driver.h"
						
UDOUBLE nMp3Size = 0;             // mp3 size
UDOUBLE nMp3Pos = 0;              // mp3 current pos
UBYTE   *nMp3name;                // mp3 playing status
UBYTE 	nAsrStatus = 0;           // Record the status of the ASR	
UBYTE 	nLD_Mode = LD_MODE_IDLE;  // Record the status of LD3320(ASR or Play Sound)
UBYTE 	bMp3Play = 0;							// Record the status of audio player
UBYTE 	ucRegVal;									// Interrupt request number
UBYTE 	ucHighInt;								// Intterrupt enable status
UBYTE 	ucLowInt;									// Intterrupt enable status
UBYTE 	ucStatus;									// FIFO status
UBYTE 	ucSPVol = 5;               // Speaker volume, MAX=15 MIN=0	

static UBYTE LD_ReadReg(UBYTE reg_add);
static UBYTE LD_AsrRun(void);
static UBYTE LD_Check_ASRBusyFlag(void);
static UBYTE LD_AsrAddFixed(void);
static void LD_ReloadMp3Data();
static void LD_Init_Common(void);
static void LD_WriteReg(UBYTE data1,UBYTE data2);
static void LD_Init_ASR(void);


/********************************************************************************
function:	
				LD3320 hardware init
********************************************************************************/
void LD_init(void)
{
  RST_1;
  Driver_Delay_ms(100);
  RST_0;
  Driver_Delay_ms(100);
  RST_1;
  
  Driver_Delay_ms(100);
  CS_0;
  Driver_Delay_ms(100);
  CS_1;   
  Driver_Delay_ms(100);
}

/********************************************************************************
  function:
        Adjust volume
  note:
        MAX=15 MIN=0
********************************************************************************/
void LD_Adjust_Volume(UBYTE val)
{
  val = ((15-val)&0x0f) << 2;
  LD_WriteReg(0x8e, val | 0xc1);  //volume
  LD_WriteReg(0x87, 0x78);    //accept adjust
}

/********************************************************************************
function:	
				Run ASR
********************************************************************************/
UBYTE LD_ASR(void)
{
	UBYTE i = 0;
	UBYTE asrflag = 0;
	for (i=0; i<5; i++) {						//run ASR try 5 times
		LD_Init_ASR();								//init ASR
		Driver_Delay_ms(100);
		if (LD_AsrAddFixed() == 0) {		//Add fixed to LD3320
			Serial.print("ERROR!!! LD_AsrAddFixed\r\n");
			LD_init();									//ERROR,Reset LD3320
			Driver_Delay_ms(50);	
			continue;
		}
		Driver_Delay_ms(10);
		if (LD_AsrRun() == 0) {				//start ASR
			Serial.print("ERROR!!! LD_AsrRun\r\n");
			LD_init();									//ERROR,Reset LD3320
			Driver_Delay_ms(50);
			continue;
		}
		asrflag = 1;
		break;						
	}
  Serial.print("RunASR\r\n");
	return asrflag;
}

/********************************************************************************
  function:
        Get LD3320 return value
********************************************************************************/
UBYTE LD_GetResult(void)
{
  return LD_ReadReg(0xc5);
}

/********************************************************************************
function:	
				Interrupt signal processing(ASR and Audio Player)
********************************************************************************/
void ProcessInt()
{
	UBYTE nAsrResCount = 0;		
	ucRegVal = LD_ReadReg(0x2B);
	ucHighInt = LD_ReadReg(0x29); // interrupt enable flag
	ucLowInt = LD_ReadReg(0x02); // interrupt enable flag
	LD_WriteReg(0x29, 0);// interrupt disenable
	LD_WriteReg(0x02, 0);// interrupt disenable
	if(nLD_Mode == LD_MODE_ASR_RUN) {
		Serial.print("---------------ASR---------------\r\n");
	  //The interruption caused by speech recognition
	  //(There is sound input, and there is interruption whether the recognition is successful or failed)
		if((ucRegVal & 0x10) && LD_ReadReg(0xb2)==0x21 && LD_ReadReg(0xbf)==0x35) {
			nAsrResCount = LD_ReadReg(0xba);
			if(nAsrResCount>0 && nAsrResCount<=4) {
				Serial.print("ASR SUCCESSFUL \r\n");
				nAsrStatus = LD_ASR_FOUNDOK;
			}
			else {
				Serial.print("ASR UNSUCCESSFUL \r\n");
				nAsrStatus = LD_ASR_FOUNDZERO;
			}	
		}
		else {
			Serial.print("No ASR \r\n");
			nAsrStatus = LD_ASR_FOUNDZERO;
		}			
		LD_WriteReg(0x2b, 0);
    LD_WriteReg(0x1c, 0);
		return;
	}	
	Serial.print("--------------PLAY MP3--------------\r\n");
	// Play MP3 to produce 3 kinkd of intterupt
	// A. play over
	// B. data send over
	// C. Data will be used up and sent
	if(LD_ReadReg(0xBA) & CAUSE_MP3_SONG_END) {
		// A. play over
		LD_WriteReg(0x2B, 0);
		LD_WriteReg(0xBA, 0);	
		LD_WriteReg(0xBC, 0);	
		LD_WriteReg(0x08, 1);
		LD_WriteReg(0x08, 0);
		LD_WriteReg(0x33, 0);
		Serial.print("play over \r\n");
		bMp3Play = 0;					// play status
		return;
	}
	if(nMp3Pos >= nMp3Size) {
		// B. data send over
		LD_WriteReg(0xBC, 0x01);//data voer
		LD_WriteReg(0x29, 0x10);
		return;	
	}
	// C. Data will be used up and sent	
	LD_ReloadMp3Data();
	LD_WriteReg(0x29, ucHighInt); 
	LD_WriteReg(0x02, ucLowInt);	
}

/********************************************************************************
function:	
				Start play
********************************************************************************/
void LD_play(UBYTE *mp3name)
{
  nMp3Pos = 0;
  bMp3Play = 1;
  nMp3name = mp3name;
  
	if(nMp3Pos >=  nMp3Size)
		return; 

	LD_ReloadMp3Data();

  LD_WriteReg(0xBA, 0x00);
	LD_WriteReg(0x17, 0x48);//activate DSP
	LD_WriteReg(0x33, 0x01);//play mp3
	LD_WriteReg(0x29, 0x04);//FIFO interrupt allowed
	LD_WriteReg(0x02, 0x01);//FIFO_DATA interrupt allowed 
	LD_WriteReg(0x85, 0x5A);
}

/********************************************************************************
function:  
        Add  ASR fixed
********************************************************************************/
static UBYTE LD_AsrAddFixed(void)
{
  UBYTE k, flag;
  UBYTE nAsrAddLength;
  #define DATE_A 5  
  #define DATE_B 20 

  UBYTE  sRecog[DATE_A][DATE_B] = {   //add commond,use pinying
        "liu shui deng",\
        "an jian",\
        "shan shuo",\
        "bo fang",\
    
                                  };  
  UBYTE  pCode[DATE_A] = {            //add commond code to do the commond
        CODE_RUN, \
        CODE_KEY, \
        CODE_FLASH,\
        CODE_PLAY,\
    
                              };  
  flag = 1;
  for(k=0; k<DATE_A; k++) {           //write data to LD3320  
    if(LD_Check_ASRBusyFlag() == 0) {
      flag = 0;
      break;
    }
    LD_WriteReg(0xc1, pCode[k]);
    LD_WriteReg(0xc3, 0);
    LD_WriteReg(0x08, 0x04);
    Driver_Delay_ms(1);
    LD_WriteReg(0x08, 0x00);
    Driver_Delay_ms(1);
    for (nAsrAddLength=0; nAsrAddLength<DATE_B; nAsrAddLength++) {
      if (sRecog[k][nAsrAddLength] == 0)
        break;
      LD_WriteReg(0x5, sRecog[k][nAsrAddLength]);
    }
    LD_WriteReg(0xb9, nAsrAddLength);
    LD_WriteReg(0xb2, 0xff);
    LD_WriteReg(0x37, 0x04);
  }  
  return flag;
}

/********************************************************************************
function:	
				Reload mp3 data
********************************************************************************/
static void LD_ReloadMp3Data()
{
	UBYTE val;
	ucStatus = LD_ReadReg(0x06);
	while (!(ucStatus&MASK_FIFO_STATUS_AFULL) && (nMp3Pos<nMp3Size)) {
		val = pgm_read_byte(&nMp3name[nMp3Pos++]);//pgm_read_byte(); --> read data from flash 
    //Serial.print("\r\nwrite mp3 data: ");
    //Serial.print(val, HEX);
		LD_WriteReg(0x01, val);
		ucStatus = LD_ReadReg(0x06);	
	}
}

/********************************************************************************
  function:
        Begin to ASR
********************************************************************************/
static UBYTE LD_AsrRun(void)
{
  LD_WriteReg(0x35, MIC_VOL);
  LD_WriteReg(0x1C, 0x09);
  LD_WriteReg(0xBD, 0x20);
  LD_WriteReg(0x08, 0x01);
  Driver_Delay_ms(20);
  LD_WriteReg(0x08, 0x00);
  Driver_Delay_ms(20);

  if (LD_Check_ASRBusyFlag() == 0)
    return 0;

  LD_WriteReg(0xB2, 0xff);
  LD_WriteReg(0x37, 0x06);
  LD_WriteReg(0x37, 0x06);
  Driver_Delay_ms(20);
  LD_WriteReg(0x1C, 0x0b);
  LD_WriteReg(0x29, 0x10);
  LD_WriteReg(0xBD, 0x00);
  return 1;
}

/********************************************************************************
  function:
        Check ASR state
********************************************************************************/
static UBYTE LD_Check_ASRBusyFlag(void)
{
  UBYTE j;
  UBYTE flag = 0;
  for (j = 0; j < 10; j++) {
    if (LD_ReadReg(0xb2) == 0x21) {
      flag = 1;
      break;
    }
    Driver_Delay_ms(10);
    //Serial.print("ERROR!!! ASR Busy\r\n");
  }
  return flag;
}

/********************************************************************************
  function:
        Common init
********************************************************************************/
static void LD_Init_Common(void)
{
  bMp3Play = 0;

  LD_ReadReg(0x06);
  LD_WriteReg(0x17, 0x35);
  Driver_Delay_ms(20);
  LD_ReadReg(0x06);

  LD_WriteReg(0x89, 0x03);
  Driver_Delay_ms(20);
  LD_WriteReg(0xCF, 0x43);
  Driver_Delay_ms(20);
  LD_WriteReg(0xCB, 0x02);

  /*PLL setting*/
  LD_WriteReg(0x11, LD_PLL_11);
  if (nLD_Mode == LD_MODE_MP3) {
    LD_WriteReg(0x1E, 0x00);
    LD_WriteReg(0x19, LD_PLL_MP3_19);
    LD_WriteReg(0x1B, LD_PLL_MP3_1B);
    LD_WriteReg(0x1D, LD_PLL_MP3_1D);
  }
  else {
    LD_WriteReg(0x1E, 0x00);
    LD_WriteReg(0x19, LD_PLL_ASR_19);
    LD_WriteReg(0x1B, LD_PLL_ASR_1B);
    LD_WriteReg(0x1D, LD_PLL_ASR_1D);
  }
  Driver_Delay_ms(20);

  LD_WriteReg(0xCD, 0x04);
  LD_WriteReg(0x17, 0x4c);
  Driver_Delay_ms(20);
  LD_WriteReg(0xB9, 0x00);
  LD_WriteReg(0xCF, 0x4F);
  LD_WriteReg(0x6F, 0xFF);
}

/********************************************************************************
  function:
        ASR init
********************************************************************************/
static void LD_Init_ASR(void)
{
  nLD_Mode = LD_MODE_ASR_RUN;
  LD_Init_Common();

  LD_WriteReg(0xBD, 0x00);
  LD_WriteReg(0x17, 0x48);
  Driver_Delay_ms(20);

  LD_WriteReg(0x3C, 0x80);
  LD_WriteReg(0x3E, 0x07);
  LD_WriteReg(0x38, 0xff);
  LD_WriteReg(0x3A, 0x07);
  Driver_Delay_ms(20);

  LD_WriteReg(0x40, 0);
  LD_WriteReg(0x42, 8);
  LD_WriteReg(0x44, 0);
  LD_WriteReg(0x46, 8);
  Driver_Delay_ms(20);
}

/********************************************************************************
  function:
        MP3 player init
********************************************************************************/
static void LD_Init_MP3()
{
  nLD_Mode = LD_MODE_MP3;
  LD_Init_Common();

  LD_WriteReg(0xBD, 0x02);
  LD_WriteReg(0x17, 0x48);
  Driver_Delay_ms(20);

  LD_WriteReg(0x85, 0x52);
  LD_WriteReg(0x8F, 0x00);
  LD_WriteReg(0x81, 0x00);
  LD_WriteReg(0x83, 0x00);
  LD_WriteReg(0x8E, 0xff);
  LD_WriteReg(0x8D, 0xff);
  Driver_Delay_ms(20);
  LD_WriteReg(0x87, 0xff);
  LD_WriteReg(0x89, 0xff);
  Driver_Delay_ms(20);
  LD_WriteReg(0x22, 0x00);
  LD_WriteReg(0x23, 0x00);
  LD_WriteReg(0x20, 0xef);
  LD_WriteReg(0x21, 0x07);
  LD_WriteReg(0x24, 0x77);
  LD_WriteReg(0x25, 0x03);
  LD_WriteReg(0x26, 0xbb);
  LD_WriteReg(0x27, 0x01);
}

/********************************************************************************
  function:
        Write data to LD3320 reg
********************************************************************************/
static void LD_WriteReg(UBYTE data1, UBYTE data2)
{
  CS_0;
  WR_0;
  spi_send_byte(0x04);
  spi_send_byte(data1);
  spi_send_byte(data2);
  CS_1;
}

/********************************************************************************
  function:
        Read data from LD3320 reg
********************************************************************************/
static UBYTE LD_ReadReg(UBYTE reg_add)
{
  UBYTE i;
  CS_0;
  WR_0;
  spi_send_byte(0x05);
  spi_send_byte(reg_add);
  i = spi_send_byte(0x00);
  CS_1;
  return (i);
}
