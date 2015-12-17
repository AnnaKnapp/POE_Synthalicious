/*
  Mega multple serial test
 
 Receives from the main serial port, sends to the others. 
 Receives from serial port 1, sends to the main serial (Serial 0).
 
 This example works only on the Arduino Mega
 
 The circuit: 
 * Any serial device attached to Serial port 1
 * Serial monitor open on Serial port 0:
 
 created 30 Dec. 2008
 modified 20 May 2012
 by Tom Igoe & Jed Roach
 
 This example code is in the public domain.
 
 */

#include <SPI.h>

#define DAC_CS          10 //pin for DAC chip select. can be changed
#define DAC_LDAC         7 //pin for load DAC, which updates the value on the DAC
#define SPICLOCK  13//sck

byte trigpin = 8; //trigger for envelope generator
byte commandByte;
byte noteByte;
byte velocityByte;
byte velocity;
byte incomingByte;
byte action;
byte notemap[128];
byte noteOn = 155;
int note;
float notesf[81];
int period[81];
int j;
byte DCO1 = 3;

void setup() {
  // initialize both serial ports:
  Serial.begin(9600);
  pinMode(13, OUTPUT);
  pinMode(3, OUTPUT);
  int lastSwitch = 0;

  
  notesf[1]= 15526.5037418;
  for (int i=1; i<81; i++){
    notesf[i+1]=notesf[i]/1.05946279638;
  }
  for (int i=1; i<81; i++){
  period[i]=int(notesf[i]-130);
  }

}

void loop() {
  // read from port 1, send to port 0:
  do {
    if (Serial.available()>0) {
      int note =Serial.read()-24;
//    Serial.print("butt");
    }
  } while ((micros()-lastSwitch) < period[note]<<1)
  
  lastSwitch = micros();
  digitalWrite(DCO1,HIGH);
  
  do {
    if (Serial.available()>0) {
      int note =Serial.read()-24;
//    Serial.print("butt");
    }
  } while ((millis()-lastSwitch) < period[note]<<1)
  digitalWrite(DCO1,LOW);

}
