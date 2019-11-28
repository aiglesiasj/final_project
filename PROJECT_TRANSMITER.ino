#include <nRF24L01.h>
#include <printf.h>
#include <RF24.h>
#include <RF24_config.h>

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

//create an RF24 object
RF24 radio(9, 8);  // CE, CSN

//address through which two modules communicate.
const byte address[6] = "00002";
int data =0;

void setup()
{
  radio.begin();
  radio.setChannel(58);
  radio.setPALevel(RF24_PA_MIN);
  radio.setDataRate (RF24_2MBPS);
  //set the address
  radio.openWritingPipe(address);
  
  //Set module as transmitter
  radio.stopListening();
}
void loop()
{
  //Send message to receiver
  const char text[] = "Hello World";
  radio.write(&data, sizeof(data));

  data++;
  
  delay(500);
}
