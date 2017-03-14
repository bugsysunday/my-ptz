#include <SoftwareSerial.h>
#define Pin13LED         8    //green
#include <printf.h>
#include <Servo.h>
SoftwareSerial portOne(1, 0); // RX, TX
Servo servo1;
Servo servo2;
int tx_led = 7;   //yellow
int sensorValue1 =0;
int sensorValue2 =0;
int sensorPin1=0;
int sensorPin2=1;
int maxPotValue = 1023;
int currentpos=1500;
int buttonPin =2;
byte byteReceived;
int byteNumber = 0;
int byteSend;
int minPulse1     =   600;   // minimum servo position
int maxPulse1     =   2400; // maximum servo position
int turnRate1  ;  // servo turn rate increment (larger value, faster rate)
int minPulse2     =   600;  // minimum servo position
int maxPulse2     =   2400; // maximum servo position
int turnRate2 ;  // servo turn rate increment (larger value, faster rate)
/** The Arduino will calculate these values for you **/
int centerServo1;
int newposX, oldposX;
int centerServo2;
int newposY, oldposY;
int pulseWidth1,currentposX;          // servo pulse width
int pulseWidth2,currentposY;          // servo pulse width
bool updatedebug= false;
const byte tiltDown = 0x10;
const byte tiltUp = 0x08;
const byte panLeft = 0x04;
const byte panRight = 0x02;

void setup(void)
  {
  pinMode(tx_led,OUTPUT);  
  Serial.begin(57600);
  printf_begin();
  //Xee= EEPROM.read(0);
  //mycam1.attach(mycam1_p);  // attaches the servo on pin 9 to the servo object X
  //mycam1.write(Xee);        //rafu runter X
  //Yee = EEPROM.read(1);      // lese x y von eeprom  
  servo1.attach(5);
  servo2.attach(6);
  centerServo1 = maxPulse1 - ((maxPulse1 - minPulse1) / 2);
  centerServo2 = maxPulse2 - ((maxPulse2 - minPulse2) / 2);
  pulseWidth1 =  centerServo1;
  pulseWidth2 =  centerServo2;
  turnRate1 = (maxPulse1- minPulse1)/720 ;
  turnRate2 = (maxPulse2- minPulse2)/720 ;  
  Serial.println("Arduino Serial Servo Control with Pelco D");
  Serial.println();
  }

void loop(void)
      {
      sensorValue1 = analogRead(sensorPin1);
      sensorValue2 = analogRead(sensorPin2);
      Serial.print("sensor1:");
      Serial.println(sensorValue1);
      Serial.print("sensor2:");
      Serial.println(sensorValue2);
      if(sensorValue1 >= 536 && sensorValue2 <= 488){
        rightup();}
      if(sensorValue1 >=536 && sensorValue2 >=536){
        rightdown();}
      if(sensorValue1 <=488 && sensorValue2 <=488){
        leftup();}
      if(sensorValue1 <=488 && sensorValue2 >=536){
        leftdown();}
      if(sensorValue1 >=536){
      panright();}
      if(sensorValue1 <= 488){
      panleft();}
      if(sensorValue2 >= 536){
      tiltup();}
      if(sensorValue2 <= 488){
        tiltdown();}
    }

      
void leftup(){
      oldposX=pulseWidth1;
      oldposY=pulseWidth2;
      newposX=pulseWidth1+turnRate1;
      newposY=pulseWidth2+turnRate2;
      if (newposX > maxPulse1){newposX = maxPulse1;}
      if (newposY < minPulse2){newposY = minPulse2;}
      while (pulseWidth1<newposX || pulseWidth2>newposY){
        if(pulseWidth1<newposX){pulseWidth1++;servo1.writeMicroseconds(pulseWidth1);}    
        if(pulseWidth2>newposY){pulseWidth2--;servo2.writeMicroseconds(pulseWidth2);}
    updatedebug = true;
    }
      delay(10);
    }
        
void rightup(){
    oldposX=pulseWidth1;
    newposX=pulseWidth1-turnRate1;
    oldposY=pulseWidth2;
    newposY=pulseWidth2-turnRate2;
    if (newposX < minPulse1){newposX = minPulse1;}
    if (newposY < minPulse2){newposY = minPulse2;}
    while(pulseWidth1>newposX ||pulseWidth2>newposY ){
      if(pulseWidth1>newposX){pulseWidth1--;servo1.writeMicroseconds(pulseWidth1);}
      if(pulseWidth2>newposY){pulseWidth2--;servo2.writeMicroseconds(pulseWidth2);}
    updatedebug = true;
    }
    delay(10);
  }
  
void leftdown(){
      oldposX=pulseWidth1;
      oldposY=pulseWidth2;
      newposX=pulseWidth1+turnRate1;
      newposY=pulseWidth2+turnRate2;
      if (newposX > maxPulse1){newposX = maxPulse1;}
      if (newposY < minPulse2){newposY = minPulse2;}
      while(pulseWidth1<newposX ||pulseWidth2<newposY){
        if(pulseWidth1<newposX){pulseWidth1++;servo1.writeMicroseconds(pulseWidth1);}
        if(pulseWidth2<newposY){pulseWidth2++;servo2.writeMicroseconds(pulseWidth2);}
      updatedebug =true;}
      delay(10);
    }
void rightdown(){
      oldposX=pulseWidth1;
      oldposY=pulseWidth2;
      newposX=pulseWidth1-turnRate1;
      newposY=pulseWidth2+turnRate2;
      if (newposX < minPulse1){newposX = minPulse1;}
      if (newposY < minPulse2){newposY = minPulse2;}
      while( pulseWidth1>newposX|| pulseWidth2<newposY){
        if(pulseWidth1>newposX){pulseWidth1--;servo1.writeMicroseconds(pulseWidth1);}
        if(pulseWidth2<newposY){pulseWidth2++;servo2.writeMicroseconds(pulseWidth2);}
        updatedebug = true;
      }
    delay(10);
  }


void panleft(){
    oldposX=pulseWidth1;
    newposX=pulseWidth1+turnRate1;
    if(newposX > maxPulse1){newposX = maxPulse1;}
      while(pulseWidth1<newposX){pulseWidth1++;servo1.writeMicroseconds(pulseWidth1);
      updatedebug = true;}
    delay(10);
  }
  
void panright(){
    oldposX=pulseWidth1;
    newposX=pulseWidth1-turnRate1;
    if (newposX < minPulse1) 
      {      
        newposX = minPulse1;
      }
    while(pulseWidth1>newposX)
      {
        pulseWidth1--;
        servo1.writeMicroseconds(pulseWidth1);
        updatedebug = true;
      }
    delay(10);
}
void tiltup(){
    oldposY=pulseWidth2;
    newposY=pulseWidth2-turnRate2;
    if (newposY < minPulse2) 
      {      
        newposY = minPulse2;
      }
    while(pulseWidth2>newposY)
      {
        pulseWidth2--;
        servo2.writeMicroseconds(pulseWidth2);
        updatedebug = true;
       }
    delay(10);
}
void tiltdown(){
    oldposY=pulseWidth2;
    newposY=pulseWidth2+turnRate2;
    if (newposY < minPulse2) 
      {      
        newposY = minPulse2;
      }
    while(pulseWidth2<newposY)
      {
        pulseWidth2++;
        servo2.writeMicroseconds(pulseWidth2);
        updatedebug = true;
      }
    delay(10);
}

void halt() {
  //sync  cam#  Command1  Command2  Data1  Data2 CheckSum
  byte  rec[7] = {0xFF, 0x01, 0x00, 0x00, 0x00, 0x00, 0x01};  //This is the most basic instruction. Camera 1, four zeros and a check sum of one
  for (int i = 0; i < 7; i++) {
    Serial.print(rec[i], DEC);
  }
}



