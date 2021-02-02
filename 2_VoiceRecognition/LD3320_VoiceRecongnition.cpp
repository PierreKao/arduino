#include "LD3320_Driver.h"
#include "LD3320_VoiceRecongnition.h"

extern UBYTE     nAsrStatus; 
VoiceRecognition::VoiceRecognition() {}

void VoiceRecognition::init()
{
    LD_Reset();
    nAsrStatus=LD_ASR_RUNING;
	  if (LD_Prepare_ASR()==ERROR)//Start the ASR process once	
		  nAsrStatus = LD_ASR_ERROR;	
}
