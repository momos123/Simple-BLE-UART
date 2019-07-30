#include <Wire.h>
#include <SPI.h>
#include <SFE_MicroOLED.h>  // Include the SFE_MicroOLED library
#include  <avr/dtostrf.h>
#include "SI114.h"
#include <BLEPeripheral.h>
#include "BLESerial.h"
BLESerial bleSerial(10, 2, 9);
unsigned long long lastSent = 0;


void setup()
{

  pinMode(6, OUTPUT);
  pinMode(18, OUTPUT);
  pinMode(17, OUTPUT);
  Wire.begin();

  bleSerial.setLocalName("UART");
  bleSerial.begin();
  Serial.begin(9600);
  Serial.println(__FILE__);

}

void loop()
{
  
  digitalWrite(6,HIGH);
  delay(500);
  digitalWrite(6,LOW);
  delay(500); // show splash for 3s
  digitalWrite(18,HIGH);
  digitalWrite(17,HIGH);
  delay(500);
  digitalWrite(17,LOW);
  digitalWrite(18,LOW);
  delay(500); // show splash for 3s

}




