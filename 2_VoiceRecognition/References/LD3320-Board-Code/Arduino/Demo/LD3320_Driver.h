/******************************************************************************
*******************************Driver for LD3320*******************************
* | file      	:	LD3320_Driver.h
* |	version			:	V1.0
* | date				:	2020-06-20
* | function		:	Driver for LD3320	
******************************************************************************/
#ifndef __LD3320_DRIVER_H
#define __LD3320_DRIVER_H

#include "LD3320_Config.h"
#include "mp3.h"

///The following five states are defined to record which state the program is in while running ASR recognition
#define LD_ASR_NONE					0x00	//Indicates that ASR recognition is not being made
#define LD_ASR_RUNING				0x01	//Indicates that LD3320 is in ASR identification
#define LD_ASR_FOUNDOK			0x10	//Represents the end of an identification process, with an identification result
#define LD_ASR_FOUNDZERO 		0x11	//Represents the end of an identification process, no identification results
#define LD_ASR_ERROR	 			0x31	//Represents an incorrect state occurring inside the LD3320 chip in a recognition process

///Identification code (Customer Modification Office)
#define CODE_RUN		1	 //commond code for LED waterfall
#define CODE_KEY	  2	 //commond code for LED flash
#define CODE_FLASH	3	 //commond code for LED reversal
#define CODE_PLAY		4	 //commond code for PLAY MP3

// LD chip fixed values.
#define        RESUM_OF_MUSIC           0x01
#define        CAUSE_MP3_SONG_END       0x20

#define        MASK_INT_SYNC						0x10
#define        MASK_INT_FIFO						0x04
#define    	   MASK_AFIFO_INT						0x01
#define        MASK_FIFO_STATUS_AFULL		0x08

///The following three states are defined to record whether the program is running ASR recognition or MP3
#define LD_MODE_IDLE      0x00
#define LD_MODE_ASR_RUN   0x08
#define LD_MODE_MP3       0x40

//LD clock set
#define CLK_IN            22        //crystal oscillator
#define LD_PLL_11         (UBYTE)((CLK_IN/2.0)-1)
#define LD_PLL_MP3_19     0x0f
#define LD_PLL_MP3_1B     0x18
#define LD_PLL_MP3_1D     (UBYTE)(((90.0*((LD_PLL_11)+1))/(CLK_IN))-1)
#define LD_PLL_ASR_19     (UBYTE)(CLK_IN*32.0/(LD_PLL_11+1) - 0.51)
#define LD_PLL_ASR_1B     0x48
#define LD_PLL_ASR_1D     0x1f

//VOL reg
#define MIC_VOL       0x43
#define SPEAKER_VOL   0x07

//LD GPIO
#define CS_0      digitalWrite(CS, LOW)
#define CS_1      digitalWrite(CS, HIGH)
#define WR_0      digitalWrite(WR, LOW)
#define WR_1      digitalWrite(WR, HIGH)
#define RST_0     digitalWrite(RST, LOW)
#define RST_1     digitalWrite(RST, HIGH)

void  ProcessInt(void);
void  LD_Adjust_Volume(UBYTE val);
UBYTE LD_GetResult(void);
UBYTE LD_ASR(void);
void  LD_init(void);
void  LD_play(UBYTE *mp3name);
void LD_Init_MP3(void);

#endif
