#include<SoftwareSerial.h>
#include<Servo.h>
#include <SimpleTimer.h>
#define rxPinPlastic 4
#define txPinPlastic 5
#define pirSensorOne 6
#define pirSensorTwo 7

SoftwareSerial arduinoCore = SoftwareSerial(rxPinPlastic,txPinPlastic);

Servo myServo;
int pos = 0;
bool isOpened = 0;

bool signalPIR = 0;
bool full = 0;

SimpleTimer timer;

void setup() {
  // put your setup code here, to run once:
  pinMode(rxPinPlastic,INPUT);
  pinMode(txPinPlastic,OUTPUT);
  pinMode(pirSensorOne,INPUT);
  pinMode(pirSensorTwo,INPUT);
  myServo.attach(9);
  timer.setInterval(10000);
  initServo();
  Serial.begin(9600);
  arduinoCore.begin(9600);
  while(!arduinoCore){;}
}

void loop() {
  // put your main code here, to run repeatedly:
  arduinoCore.listen();
  if(arduinoCore.available()){
    char c = arduinoCore.read();
    if(c=='1'){
      openBasket();
    }
  }
}

void openBasket(){
  if(isOpened==0){
    for(pos=1; pos<=130; pos+=1){
      myServo.write(pos);
      delay(20);
    }
    isOpened = 1;
    checkTrash();
  }
}

void closeBasket(){
  if(isOpened==1){
    for(pos=130; pos>=1; pos-=1){
    myServo.write(pos);
    delay(20);
  }
    isOpened = 0;
    arduinoCore.print(full);
  }
}

void initServo(){
  myServo.write(1);
  delay(100);
}

void checkTrash(){
  timer.reset();
  while(timer.isReady()==0){
    signalPIR = getSignalFromSensors();
    if(signalPIR==1){
      timer.reset();
      delay(3000);
      break;
    }
    delay(50);
  }
  signalPIR = 0;
  closeBasket();
}

bool getSignalFromSensors(){
  bool sensorValue = 0;
  if(digitalRead(pirSensorOne)==1){
     sensorValue = 1;
     for(int k=0; k<1499; k++){
      if(digitalRead(pirSensorTwo)==1){
        full=0;
        break;
      }
      full=1;
      delay(1);
     }
  }else if(digitalRead(pirSensorTwo)==1){
    sensorValue = 1;
    full = 0;
  }
  return sensorValue;
}
