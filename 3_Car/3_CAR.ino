#include <IRremote.h>

//Key Mapping
#define PIN_MOTOR_A1 8 // A_1A控制A馬達的前進（HIGH）、後退（LOW）
#define PIN_MOTOR_A2 9 // A_1B控制A馬達的速度 0~255 ，LOW表示停止
#define PIN_MOTOR_B1 10 // B_1A控制B馬達的前進（HIGH）、後退（LOW）
#define PIN_MOTOR_B2 11 // B_1B控制B馬達的速度 0~255，LOW表示停止
#define Speed 150 //設定前進速度

// command key value define
#define KEY_PWR     0x45
#define KEY_MOD     0x46
#define KEY_MUTE    0x47
#define KEY_PLAY    0x44
#define KEY_PRE     0x40
#define KEY_NEXT    0x43
#define KEY_EQ      0x07
#define KEY_VOL_D   0x15
#define KEY_VOL_U   0x09
#define KEY_REPEAT  0x19
#define KEY_SD      0x0D
#define KEY_0       0x16
#define KEY_1       0x0C
#define KEY_2       0x18
#define KEY_3       0x5E
#define KEY_4       0x08
#define KEY_5       0x1C
#define KEY_6       0x5A
#define KEY_7       0x42
#define KEY_8       0x52
#define KEY_9       0x4A

int RECV_PIN = 2; // 使用數位腳位2接收紅外線訊號

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);  
  IrReceiver.begin(RECV_PIN, false);  

  pinMode(PIN_MOTOR_A1,OUTPUT);
  pinMode(PIN_MOTOR_A2,OUTPUT);
  pinMode(PIN_MOTOR_B1,OUTPUT);
  pinMode(PIN_MOTOR_B2,OUTPUT);
  digitalWrite(PIN_MOTOR_A1,LOW);
  digitalWrite(PIN_MOTOR_A2,LOW);
  digitalWrite(PIN_MOTOR_B1,LOW);
  digitalWrite(PIN_MOTOR_B2,LOW);
}

void loop() {
  // put your main code here, to run repeatedly:  
  if (IrReceiver.decode()) {    
    //IrReceiver.printIRResultShort(&Serial);    
    //Serial.println();    
    Serial.print("results value is "); // 輸出解碼後的資料
    Serial.print(IrReceiver.decodedIRData.command, HEX);
    Serial.println();
    switch (IrReceiver.decodedIRData.command)
    {
      case KEY_PWR:
        stopCar();
        delay(2000);
        break;
      case KEY_MOD:        
        moveForward();
        delay(2000);
        break;
      case KEY_PLAY:
        turnLeft();
        delay(2000);
        break;
      case KEY_VOL_D:
        moveBackward();
        delay(2000);
        break;
      case KEY_NEXT:
        turnRight();
        delay(2000);
        break;
    default:
      break;
    }
    IrReceiver.resume(); // 準備接收下一個訊號  
  }
  delay(100);
}

void stopCar(){
    digitalWrite(PIN_MOTOR_A1,LOW);
    digitalWrite(PIN_MOTOR_A2,LOW);
    digitalWrite(PIN_MOTOR_B1,LOW);
    digitalWrite(PIN_MOTOR_B2,LOW);
}

void moveForward(){
  Serial.println("moveForward");
  digitalWrite(PIN_MOTOR_A1,HIGH);
  analogWrite(PIN_MOTOR_A2,Speed);
  digitalWrite(PIN_MOTOR_B1,HIGH);
  analogWrite(PIN_MOTOR_B2,Speed);
  return;
}

void moveBackward(){
  Serial.println("moveBackward");
  digitalWrite(PIN_MOTOR_A1,LOW);
  analogWrite(PIN_MOTOR_A2,Speed);
  digitalWrite(PIN_MOTOR_B1,LOW);
  analogWrite(PIN_MOTOR_B2,Speed);
  return;
}

void turnRight(){
  Serial.println("turnRight");
  digitalWrite(PIN_MOTOR_A1,LOW);
  analogWrite(PIN_MOTOR_A2,LOW);
  digitalWrite(PIN_MOTOR_B1,HIGH);
  analogWrite(PIN_MOTOR_B2,Speed);
  return;
}

void turnLeft(){
  Serial.println("turnLeft");
  digitalWrite(PIN_MOTOR_A1,HIGH);
  analogWrite(PIN_MOTOR_A2,Speed);
  digitalWrite(PIN_MOTOR_B1,LOW);
  analogWrite(PIN_MOTOR_B2,LOW);
  return;
}