#include "LD3320_VoiceRecongnition.h"

VoiceRecognition gVoiceRecognition;


void setup() {
  // // put your setup code here, to run once:  
  Serial.print("---------------LD3320 DEMO---------------\r\n");
  gVoiceRecognition.init();
  gVoiceRecognition.addCommand("kai deng", 0);
  gVoiceRecognition.addCommand("guan deng", 1);
  //gVoiceRecognition.start();
}

void loop() {
  // put your main code here, to run repeatedly:
  switch (gVoiceRecognition.read()) //判断识别
  {
    case 0:                    //若是指令“kai deng”    
      Serial.print("--------------kai deng---------------\r\n");
      break;      
    case 1:                   //若是指令“guan deng”    
      Serial.print("--------------guan deng---------------\r\n");
      break;
    default:
      break;
  }
  //reset for next run
}


