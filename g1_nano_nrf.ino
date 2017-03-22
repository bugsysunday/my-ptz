#include <RF24Network.h>
#include <RF24.h>
#include <SPI.h>
#include <printf.h>
#include <Servo.h>

#define NetLED         4
const int good_led = 3;
Servo servo1;
const int servoPin1=5;
Servo servo2;
const int servoPin2=6;  


int maxPotValue = 1023;
int currentpos=1500;
const int buttonPin =2;
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
byte rx_byte= 0;



RF24 radio(7,8);                // nRF24L01(+) radio attached using Getting Started board 

RF24Network network(radio);      // Network uses that radio
const uint16_t ptz1 = 01;    // Address of our node in Octal format ( 04,031, etc)
const uint16_t ctrl = 00;   // Address of the other node in Octal format

struct payload_t {                 // Structure of our payload
  unsigned long ms;
  byte ptz;
  };

void setup()  {
    Serial.begin(9600);
    Serial.println("Arduino pantilt_unit  nano");
    printf_begin();
    Serial.println("RF24Network/NANO/google_1_ptz/");
    pinMode(NetLED, OUTPUT);   
    SPI.begin();
    Serial.println("spi init");
    radio.begin();
    Serial.println("radio init");
    radio.setPALevel(RF24_PA_HIGH);
    radio.setDataRate(RF24_250KBPS);
    //  radio.setAutoAck(1);                       // Ensure autoACK is enabled
    radio.setRetries(7,15);                    // Optionally, increase the delay between retries & # of retries
    radio.setCRCLength(RF24_CRC_8);
    Serial.println("radio tweaks set");
    
    network.begin(/*channel*/ 90, /*node address*/ ptz1);
    Serial.println("net init");  
    radio.printDetails();
    servo1.attach(servoPin1);
    Serial.println("servo1 attached ");
    servo2.attach(servoPin2);
    Serial.println("servo2 attached ");
    centerServo1 = maxPulse1 - ((maxPulse1 - minPulse1) / 2);
    centerServo2 = maxPulse2 - ((maxPulse2 - minPulse2) / 2);
    pulseWidth1 =  centerServo1;
    pulseWidth2 =  centerServo2;
    turnRate1 = (maxPulse1- minPulse1)/720 ;
    turnRate2 = (maxPulse2- minPulse2)/720 ;  
  }

void loop(){
              network.update();                  // Check the network regularly
              while ( network.available()){     // Is there anything ready for us?
              digitalWrite(NetLED, HIGH);  // Show activity
              RF24NetworkHeader header(ctrl);        // If so, grab it and print it out
              payload_t payload;
              network.read(header,&payload,sizeof(payload));
               Serial.println(payload.ptz);
                  switch (payload.ptz){
                      case 1:    // your hand is on the sensor
                        //if(sensorValue1 <=488 && sensorValue2 >=536){
                        leftdown();
                        Serial.println("leftdown");
                        break;
                      case 2:    // your hand is close to the sensor
                        //if(sensorValue2 <= 488){
                        tiltdown();
                        Serial.println("tiltdown");
                        break;
                      case 3:    // your hand is a few inches from the sensor
                        //if(sensorValue1 >=536 && sensorValue2 >=536){
                        rightdown();
                        Serial.println("rightdown");
                        break;
                      case 4:    // your hand is nowhere near the sensor
                        //if(sensorValue1 <= 488){
                        panleft();
                        Serial.println("panleft");
                        break;
                      case 6:    // your hand is nowhere near the sensor
                        //if(sensorValue1 >=536){
                        panright();
                        Serial.println("panright");
                        break;
                      case 7:    // your hand is nowhere near the sensor
                        //if(sensorValue1 <=488 && sensorValue2 <=488){
                        leftup();
                        Serial.println("leftup");
                        break;
                      case 8:    // your hand is nowhere near the sensor
                        //if(sensorValue2 >= 536){
                        tiltup();
                        Serial.println("tiltup");
                        break;
                      case 9:    // your hand is nowhere near the sensor
                        //if(sensorValue1 >= 536 && sensorValue2 <= 488){
                        rightup();
                        Serial.println("rightup");
                        break;  
                      case 0:    // your hand is nowhere near the sensor
                        //if(sensorValue1 >= 536 && sensorValue2 <= 488){
                        Serial.println("halt");
                        break;        
                      default:    // your hand is nowhere near the sensor
                        Serial.println("halt");
                        break;           
                      }
                      Serial.print("Received packet #");
                      Serial.println(payload.ptz);
                      Serial.print(" at ");
                      Serial.println(payload.ms);
                     // payload.ptz=0;
                      }
                 digitalWrite(NetLED, LOW);  // Show activity
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
      }
      delay(10);   
      updatedebug = true;
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
    }    
    updatedebug = true;
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
      }
      delay(10);  
      updatedebug =true;
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
      }
    delay(10);
    updatedebug = true;
}


void panleft(){
    oldposX=pulseWidth1;
    newposX=pulseWidth1+turnRate1;
    if(newposX > maxPulse1){newposX = maxPulse1;}
      while(pulseWidth1<newposX){pulseWidth1++;servo1.writeMicroseconds(pulseWidth1);}
    updatedebug = true;
    delay(10);
  }
  
void panright(){
    oldposX=pulseWidth1;
    newposX=pulseWidth1-turnRate1;
    if (newposX < minPulse1){newposX = minPulse1;}
    while(pulseWidth1>newposX){pulseWidth1--;servo1.writeMicroseconds(pulseWidth1);}
    updatedebug = true;
    delay(10);
}
void tiltup(){
    oldposY=pulseWidth2;
    newposY=pulseWidth2-turnRate2;
    if (newposY < minPulse2){ newposY = minPulse2; }
      while(pulseWidth2>newposY) {pulseWidth2--; servo2.writeMicroseconds(pulseWidth2);}
      updatedebug = true;
      delay(10);
}
void tiltdown(){
    oldposY=pulseWidth2;
    newposY=pulseWidth2+turnRate2;
    if (newposY < minPulse2)    {    newposY = minPulse2;  }
       while(pulseWidth2<newposY){pulseWidth2++;servo2.writeMicroseconds(pulseWidth2);}
    updatedebug = true;
    delay(10);
}
