#include "LD3320_Demo.h"

void setup() {
  // put your setup code here, to run once:
  System_Init();
  attachInterrupt(0, blink, FALLING);
  Serial.print("---------------LD3320 DEMO---------------\r\n");
  LD3320_test();
}

void loop() {
  // put your main code here, to run repeatedly:

}

void blink() {
  ProcessInt();
}
