#include <nRF24L01.h>
#include <printf.h>
#include <RF24.h>
#include <RF24_config.h>

//Include Libraries
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

//create an RF24 object
RF24 radio(9, 8);  // CE, CSN

//address through which two modules communicate.
const byte address[6] = "00002";
int lastV;
unsigned long t;
int level;
boolean isOn = false;

boolean latido = false;
unsigned long tLatido;

void setup()
{
  pinMode(3, OUTPUT);
  digitalWrite(3, HIGH);

  while (!Serial);
  Serial.begin(9600);

  radio.begin();
  radio.setChannel(58);
  radio.setPALevel(RF24_PA_MIN);
  radio.setDataRate(RF24_2MBPS);

  //set the address
  radio.openReadingPipe(0, address);

  //Set module as receiver
  radio.startListening();
}

void latido2 () {

  if (millis() - tLatido > 300 * (4 - level)) // latido depends of the level
  {
    if (latido) {
      digitalWrite(3, HIGH); //turn OFF the VIBRATION MOTOR
    } else {
      digitalWrite(3, LOW); //turn ON the VIBRATION MOTOR
    }
    latido = !latido;
    tLatido = millis();
  }
  if (level == 0) {
    digitalWrite(3, HIGH); //turn OFF the VIBRATION MOTOR
    latido = false;
  }

}


void loop()
{
  if (analogRead(A0) > 300) {
    isOn = !isOn;
    while (analogRead(A0) > 300) {}
  }

  if (isOn) {
    //Read the data if available in buffer
    if (radio.available())
    {
      int data = 0;
      radio.read(&data, sizeof(data));
      t = millis();
      //Serial.println(data);

      if (data - lastV == 1) { //if any message is lost, level 3
        level = 3;

      } else if (data - lastV < 4) { //if <4 messages are lost, level 2
        level = 2;

      } else if (data - lastV < 7) { //if <7 messages are lost, level 3
        level = 1;

      }
      lastV = data;
    }

    if (millis() - t > 5000) {
      level = 0;
    }

    Serial.println(level);
    

    latido2(); //function latido2
  } else {
    digitalWrite(3, HIGH); //turn OFF the VIBRATION MOTOR
    latido = false;
  }
}
