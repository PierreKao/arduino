#include "LD3320_VoiceRecongnition.h"

VoiceRecognition gVoiceRecognition;

void setup() {
  // // put your setup code here, to run once:  
  gVoiceRecognition.init();
  gVoiceRecognition.addCommand(NULL,NULL);
}

void loop() {
  // put your main code here, to run repeatedly:
  switch (gVoiceRecognition.read()) //判断识别
  {
    case 0:                    //若是指令“kai deng”    
      break;
    case 1:                   //若是指令“guan deng”    
      break;
    case 2:                   //若是指令“guan deng”    
      break;
    default:
      break;
  }

  //reset for next run
}


