#include <SPI.h>
#include <MFRC522.h>

//RFID-RF522 Pin Define
#define RST_PIN 9 //Reset
#define SS_PIN 10 //SDA

//Buzzer Pin Define    
#define BUZZER_PIN 7

//RGB Light Pin Define 
#define RED_PIN 6      
#define GREEN_PIN 5    
#define BLUE_PIN 3     
MFRC522 mfrc522;

void setup() {         
  // put your setup code here, to run once:
  
  //Setup serial port
  Serial.begin(9600);
  
  //Setup MFRC522 Module
  //Initial SPI                   
  SPI.begin();  
  //Initial MFRC522 module
  mfrc522.PCD_Init(SS_PIN, RST_PIN);  
  Serial.print(F("Reader "));                                   
  Serial.print(F(": "));             
  //Dump module version                           
  mfrc522.PCD_DumpVersionToSerial(); 
  
  //Setup Buzzer                                                
  pinMode(BUZZER_PIN,OUTPUT);
  
  //Setup RGB LED                                               
  pinMode(RED_PIN, OUTPUT);                                     
  pinMode(GREEN_PIN, OUTPUT);                                   
  pinMode(BLUE_PIN, OUTPUT);                                    
}

void loop() {

  // put your main code here, to run repeatedly:
  
  //Setup LED color as RED until card new card detected  
  analogWrite(RED_PIN, 255);
  
  //Module detect new card and read UID  
  if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) { 
    Serial.print(F("Card UID:"));    
    Serial.println();                
    // Get and print card(picc) type    
    Serial.print(F("PICC type: "));     
    MFRC522::PICC_Type piccType = mfrc522.PICC_GetType(mfrc522.uid.sak);    
    Serial.println(mfrc522.PICC_GetTypeName(piccType));
    
    //Change RGB LED color to green                                         
    analogWrite(RED_PIN, 0);                                                
    analogWrite(GREEN_PIN, 255);
    
    //Play buzzer                                                           
    digitalWrite(BUZZER_PIN, HIGH);                                         
    delay(500);
    
    //Change RGB LED color to red    
    analogWrite(RED_PIN, 255);    
    analogWrite(GREEN_PIN, 0);
    
    //Stop buzzer                    
    digitalWrite(BUZZER_PIN, LOW);
    
    //Card(picc) enter halt mode
    mfrc522.PICC_HaltA();  
  }
}

