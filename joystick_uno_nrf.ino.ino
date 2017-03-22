
#include <RF24Network.h>
#include <RF24.h>
#include <SPI.h>
#include <printf.h>

#define NetLED         3

const int txLED = 4;

RF24 radio(9,10);                    // nRF24L01(+) radio attached using Getting Started board 
RF24Network network(radio);          // Network uses that radio


const uint16_t ctrl = 00;        // Address of our node in Octal format
const uint16_t ptz1 = 01;       // Address of the other node in Octal format

unsigned long interval = 200; //ms  // How often to send 'hello world to the other unit

unsigned long last_sent;             // When did we last send?
unsigned long packets_sent;          // How many have we sent already


struct payload_t {                  // Structure of our payload
  unsigned long ms;
  byte ptz;
  };

bool sensor_update = false;
byte tx_byte;
bool debug =false;
int sensorValue1 =0;
int sensorValue2 =0;
int sensorPin1=0;
int sensorPin2=1;
int recv1;

// software serial #2: RX = digital pin 8, TX = digital pin 9
// on the Mega, use other pins instead, since 8 and 9 don't work on the Mega

void setup()
    {
      Serial.begin(9600); //Open serial communications and wait for port to open:
      printf_begin();
      Serial.println("RF24Network/examples/helloworld_tx/");
      pinMode(NetLED, OUTPUT);   
      pinMode(txLED, OUTPUT);   
      SPI.begin();
      radio.begin();
     radio.setPALevel(RF24_PA_HIGH);
     radio.setDataRate(RF24_250KBPS);
    //  radio.setAutoAck(1);                       // Ensure autoACK is enabled
     radio.setRetries(7,15);                    // Optionally, increase the delay between retries & # of retries
     radio.setCRCLength(RF24_CRC_8);
      network.begin(/*channel*/ 90, /*node address*/ ctrl);
      radio.printDetails();
     /* Serial.println("Hhardareserial initalized");
      // Start each software serial port
      portOne.begin(115200);
      Serial.println("portOneSerial init");
      establishContact();
    */
    }

void loop() {
      network.update();                          // Check the network regularly
      unsigned long now = millis();              // If it's time to send a message, send it!
      if (now - last_sent >= interval){
          sensorValue1 = analogRead(sensorPin1);
          sensorValue2 = analogRead(sensorPin2);
          if(debug){
              Serial.print("sensor1:");
              Serial.println(sensorValue1);
              Serial.print("sensor2:");
              Serial.println(sensorValue2);
            }
          if(sensorValue1 >= 556 && sensorValue2 <= 462 && !sensor_update){
            tx_byte=9;
            sensor_update=true;
            }
          if(sensorValue1 >=556 && sensorValue2 >=556 && !sensor_update ){
            tx_byte=3;
            sensor_update=true;
            }
          if(sensorValue1 <=462 && sensorValue2 <=462 &&!sensor_update){
            tx_byte=7;
            sensor_update=true;
            }
          if(sensorValue1 <=462 && sensorValue2 >=556 && !sensor_update){
            tx_byte=1;
            sensor_update=true;
            }
          if(sensorValue1 >=556 && !sensor_update){
            tx_byte=6;
            sensor_update=true;
            }
          if(sensorValue1 <= 462 && !sensor_update){
            tx_byte=4;
            sensor_update=true;
            }
          if(sensorValue2 >= 556 && !sensor_update){
            tx_byte=8;
            sensor_update=true;
            }
          if(sensorValue2 <= 462 && !sensor_update){
            tx_byte=2;
            sensor_update=true;
            }
          if(sensor_update){
  
          last_sent = now;
          Serial.println("Sending...");
          RF24NetworkHeader header(/*to node*/ ptz1);
          payload_t payload = { millis(), tx_byte };
          bool ok = network.write(header,&payload,sizeof(payload));
          if (ok)
          {
            Serial.println("ok.");
            digitalWrite(txLED,HIGH);
          }
            else
         {
           Serial.println("failed.");
           digitalWrite(txLED,LOW);
         }
              Serial.println("tx_byte: ");
      Serial.println(tx_byte);
         sensor_update=false;
      payload.ptz=0;

          }
 
    } 
}

 

    

/*
void establishContact() {
  while (portOne.available() <= 0) {
    portOne.print('0');   // send a capital A
    delay(300);
  }
}
*/


