
/*  WHAT IT DOES: Reads Analog values and transmit its coordinate over a nRF24L01 Radio Link to another transceiver.
wire them according to nRF24L01's Pin Array, 
1 - GND
2 - VCC 3.3V !!! NOT 5V
3 - CE to Arduino pin 9
4 - CSN to Arduino pin 10
5 - SCK to Arduino pin 13
6 - MOSI to Arduino pin 11
7 - MISO to Arduino pin 12
8 - UNUSED

- Analog Joystick:
GND to Arduino GND
VCC to Arduino +5V
X Pot to Arduino A0
Y Pot to Arduino A1
P Pot to Arduino A2
T Pot to Arduino A3
Flipper Pot to Arduino A4
Flipper2 Pot to Arduino A5
*/
/*-----( Import needed libraries )-----*/
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
/*-----( Declare Constants and Pin Numbers )-----*/

#define CE_PIN 9
#define CSN_PIN 10

#define JOYSTICK_X A0
#define JOYSTICK_Y A1
#define JOYSTICK_P A2
#define JOYSTICK_T A3
#define JOYSTICK_Fliper A4
#define JOYSTICK_Fliper2 A5
// NOTE: the "LL" at the end of the constant is "LongLong" type
int Red_button = 6;
int Blue_button =7; 
//int msg[1];
int val_1;  
int val_2; 
const uint64_t pipe = 0xE8E8F0F0E1LL; 
// Define the transmit pipe
/*-----( Declare objects )-----*/
RF24 radio(CE_PIN, CSN_PIN); // Create a Radio
/*-----( Declare Variables )-----*/
int joystick[8]; // 2 element array holding Joystick readings
void setup() 
/****** SETUP: RUNS ONCE ******/
{
     Serial.begin(9600);
     radio.begin();
     radio.openWritingPipe(pipe);
     pinMode(Red_button,INPUT_PULLUP); 
    //digitalWrite(Red_button,LOW); 
     pinMode(Blue_button,INPUT_PULLUP);
  // digitalWrite(Blue_button,LOW);   
}//--(end setup )---
/****** LOOP: RUNS CONSTANTLY ******/
void loop() 
{      
       joystick[0] = analogRead(JOYSTICK_X); //drive wheel1
       joystick[1] = analogRead(JOYSTICK_Y); //drive wheel2
       joystick[2] = analogRead(JOYSTICK_P);//turret X-axis
       joystick[3] = analogRead(JOYSTICK_T);//turret Y-axis
       joystick[4] = analogRead(JOYSTICK_Fliper);//for left variable tread
       joystick[5] = analogRead(JOYSTICK_Fliper2);//for right variable tread
       joystick[6]=  digitalRead(Red_button); //red laser 
       joystick[7]=  digitalRead(Blue_button);//blue laser
       radio.write(&joystick, sizeof(joystick) ); //transmit coordinate to receiver
}//--(end main loop )--
