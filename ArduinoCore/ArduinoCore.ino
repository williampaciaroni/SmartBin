#include<SoftwareSerial.h>
#include <ArduinoJson.h>
#include <SimpleTimer.h>

#define rxPinPlastic 4
#define txPinPlastic 5
#define rxPinPaper 6
#define txPinPaper 7
#define rxPinQR 8
#define txPinQR 9
#define rxPinBLEModule 11
#define txPinBLEModule 12
#define qrControl 13

SoftwareSerial arduinoPlasticArm = SoftwareSerial(rxPinPlastic,txPinPlastic);

SoftwareSerial arduinoPaperArm = SoftwareSerial(rxPinPaper,txPinPaper);
bool plasticStatus = 0;
bool paperStatus = 0;


SoftwareSerial qrSerial=SoftwareSerial(rxPinQR,txPinQR);
bool isQrRead = 0;

SoftwareSerial BLEModule=SoftwareSerial(rxPinBLEModule,txPinBLEModule);
bool btStatus = 0;

StaticJsonDocument<512> jsonDoc;


SimpleTimer timer;

void setup() {
  // put your setup code here, to run once:
  pinMode(rxPinPlastic,INPUT);
  pinMode(txPinPlastic,OUTPUT);
  pinMode(rxPinPaper,INPUT);
  pinMode(txPinPaper,OUTPUT);
  pinMode(rxPinQR,INPUT);
  pinMode(txPinQR,OUTPUT);
  pinMode(rxPinBLEModule,INPUT);
  pinMode(txPinBLEModule,OUTPUT);
  pinMode(qrControl,OUTPUT);
  Serial.begin(9600);
  while(!Serial){;}
  arduinoPlasticArm.begin(9600);
  while(!arduinoPlasticArm){;}
  arduinoPaperArm.begin(9600);
  while(!arduinoPlasticArm){;}
  qrSerial.begin(57600);
  while(!qrSerial){;}
  BLEModule.begin(9600);
  while(!BLEModule){;}
  timer.setInterval(30000);
}

void loop() {
  // put your main code here, to run repeatedly:
  if(btStatus==0){
    digitalWrite(qrControl,LOW);
    BLEModule.listen();
    char c = ' ';
    while(BLEModule.available()>0 || c==' '){
      char s = BLEModule.read();
      if(s=='o'){
        break;
      }
    }
    btStatus=1;
    timer.reset();
  }else{
    if(isQrRead==0){
        digitalWrite(qrControl,HIGH);
        if(qrSerial.isListening()==0){
          qrSerial.listen();
        }
        String s = "";
        while (qrSerial.available() > 0 && timer.isReady()==0) {
          char c = qrSerial.read();
          s+=c;
        }
        if(timer.isReady()==1){
            btStatus = 0;
        }
        if(s!="" || s.charAt(0)==' '){
                        if(isValid(s)==1){                        
                          isQrRead==1;
                          digitalWrite(qrControl,LOW);
                          getMaterial();    
                        }
        }
      }
  }
  
}

bool isValid(String qrCode){
    DeserializationError error = deserializeJson(jsonDoc, qrCode);
    String basket = jsonDoc["basket"];
    if(basket.equals("plastic") || basket.equals("paper")){
      return 1;
    }else{
      return 0;
    }
}

void getMaterial(){
    String basket = jsonDoc["basket"];
    if(basket.equals("plastic")){
      openPlasticBasket();
    }else if(basket.equals("paper")){
      openPaperBasket();
    }
}

void openPlasticBasket(){
  arduinoPlasticArm.print(1);
  arduinoPlasticArm.listen();
  char c=' ';
  while(c==' '){
    if(arduinoPlasticArm.available()){
      c = arduinoPlasticArm.read();
    }
  }
  Serial.println(c);
  plasticStatus=c;
  BLEPrint();
  btStatus=0;
  isQrRead=0;
}
void openPaperBasket(){
  arduinoPaperArm.print(1);
  arduinoPaperArm.listen();
  char c=' ';
  while(c==' '){
    if(arduinoPaperArm.available()){
      c = arduinoPaperArm.read();
    }
  }
  Serial.println(c);
  paperStatus=c;
  BLEPrint();
  btStatus=0;
  isQrRead=0;
}

void BLEPrint(){
  BLEModule.print('p');
  delay(10);
  BLEModule.print(plasticStatus);
  delay(10);
  BLEModule.print('c');
  delay(10);
  BLEModule.print(paperStatus);
  delay(10);
  BLEModule.print('\n');
}
