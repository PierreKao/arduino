#include "LD3320_Driver.h"
#include "LD3320_VoiceRecongnition.h"

#define MAX_KEY_LEN 20

extern UBYTE     nAsrStatus;

VoiceRecognition::VoiceRecognition() {}

void VoiceRecognition::init()
{
    System_Init();
    attachInterrupt(0, ProcessInt, FALLING);    
    LD_Reset();    
}

void VoiceRecognition::start()
{
    LD_Start_ASR();
}

int VoiceRecognition::addCommand(char *pass, int num)
{
    UBYTE k, flag;
    UBYTE nAsrAddLength;

    flag = SUCCESS;

    if (LD_Is_ASR_Busy() == ERROR)
    {
        return ERROR;
    }
    LD_WriteReg(0xc1, num);
    LD_WriteReg(0xc3, 0);
    LD_WriteReg(0x08, 0x04);
    Driver_Delay_ms(1);
    LD_WriteReg(0x08, 0x00);
    Driver_Delay_ms(1);

    for (nAsrAddLength = 0; nAsrAddLength <= MAX_KEY_LEN; nAsrAddLength++)
    {
        if (pass[nAsrAddLength] == 0)
            break;
        LD_WriteReg(0x5, pass[nAsrAddLength]);
    }
    LD_WriteReg(0xb9, nAsrAddLength);
    LD_WriteReg(0xb2, 0xff);
    LD_WriteReg(0x37, 0x04);

    return flag;
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
