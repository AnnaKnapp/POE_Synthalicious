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
float noteCvF[81];
int noteCv[81];
int j;
void DACwriteChannel(byte ch, int value) {
  if ((ch<2) && (value>=0) && (value<4096)) {
    digitalWrite(DAC_CS, LOW);
    SPI.transfer(((ch==0) ? 0x30:0xB0)|(highByte(value)&0x0F));
    SPI.transfer(lowByte(value));
    digitalWrite(DAC_CS, HIGH);
    digitalWrite(DAC_LDAC, LOW);
    digitalWrite(DAC_LDAC, HIGH);
  }
}

void DACwriteChannels(int value0, int value1) {
  if ((value0>=0) && (value0<4096) && (value1>=0) && (value1<4096)) {
    digitalWrite(DAC_CS, LOW);
    SPI.transfer(0x30|(highByte(value0)&0x0F));
    SPI.transfer(lowByte(value0));
    digitalWrite(DAC_CS, HIGH);
    digitalWrite(DAC_CS, LOW);
    SPI.transfer(0xB0|(highByte(value1)&0x0F));
    SPI.transfer(lowByte(value1));
    digitalWrite(DAC_CS, HIGH);
    digitalWrite(DAC_LDAC, LOW);
    digitalWrite(DAC_LDAC, HIGH);
  }
}

void setup() {
  // initialize both serial ports:
  Serial.begin(9600);
  pinMode(13, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(DAC_CS, OUTPUT);
  digitalWrite(DAC_CS, HIGH);
  pinMode(DAC_LDAC, OUTPUT);
  digitalWrite(DAC_LDAC, HIGH);
  pinMode(SPICLOCK,OUTPUT);

  
  
  notesf[1]= 15526.5037418;
  for (int i=1; i<60; i++){
    notesf[i+1]=notesf[i]/1.05946279638;
  }
  for (int i=1; i<60; i++){
  note[i]=int(notesf[i]-130);
  }

}

void loop() {
  // read from port 1, send to port 0:
  
  if (Serial.available()) {
    digitalWrite(13,LOW);
    int note =Serial.read();
    if (note>0){
    digitalWrite(13, HIGH);
    analogWrite(3, note);
    DACwriteChannel(0, noteCv[note-24]);
    DACwriteChannel(1, noteCv[note-24]);
    }
    else if (note==0) {
    digitalWrite(13, LOW);
    analogWrite(3, 0);
    DACwriteChannel(0, 0);
    DACwriteChannel(1, 0);
    } 
  }
  else{
  }
}
