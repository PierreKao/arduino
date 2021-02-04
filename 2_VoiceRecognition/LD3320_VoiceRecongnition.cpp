#include "LD3320_Driver.h"
#include "LD3320_VoiceRecongnition.h"

extern UBYTE     nAsrStatus; 
VoiceRecognition::VoiceRecognition() {}

void VoiceRecognition::init()
{
    System_Init();
    attachInterrupt(0, ProcessInt, FALLING);
    Serial.print("---------------LD3320 DEMO---------------\r\n");  
    LD_Reset();
}

void VoiceRecognition::addCommand(char *pass, int num)
{

}

int VoiceRecognition::read()
{
    switch (nAsrStatus)
    {
        case LD_ASR_RUNING:
        case LD_ASR_ERROR:
            break;
        case LD_ASR_NONE:
            nAsrStatus = LD_ASR_RUNING;
            if (LD_Prepare_ASR() == 0) //Start the ASR process once
                nAsrStatus = LD_ASR_ERROR;
            break;
        case LD_ASR_FOUNDOK:        
            nAsrStatus = LD_ASR_NONE;
            return LD_GetResult(); //once ASR process end, get the result        
        case LD_ASR_FOUNDZERO:
        default:
            nAsrStatus = LD_ASR_NONE;
            break;
    }
}
