#include <SPI.h>
#include <Servo.h>

#include <nRF24L01.h>
#include <RF24.h>
#include <RF24_config.h>

/* 송신기에서 조이스틱의 값을 받아 출력함. 수신기임!!
pin array is silmillar with trans.ino but reference arduino mega's SPI communication.

#define CE_PIN 11 //CE
#define CSN_PIN 12 //CSN 통신용 PWM

const uint64_t pipe = 0xE8E8F0F0E1LL; // 전송파이프 정의
/*-----( 오브젝트정의 )-----*/
RF24 radio(CE_PIN, CSN_PIN); // 라디오생성, 9,10핀의 값을 CE와 CSN으로 받음
/*-----( Declare Variables )-----*/
int joystick[9]; // 조이스틱값받는배열. 셋업참조.
//서보 추가시작
int msg[1];
int servoPanPosition = 90;
int servoTiltPosition = 90;
int joystickPanSpeed = 0;
int joystickTiltSpeed = 0;
//
int Red_button =13;
int Blue_button =48; 
int pos = 0 ;
Servo servo1;
Servo servo2;
//끝
//
const int motorFliper2_A =2; // (pwm) for transformable tread.  
const int motorFliper2_B = 3;  // 
const int motorFliper_A =8; // 
const int motorFliper_B = 9;  
const int motorA_A = 4;  // (pwm) pin 4 connected to pin A-IA (drive)
const int motorA_B = 5;  // (pwm) pin 5 connected to pin A-IB 
const int motorB_A =6; // (pwm) pin 6 connected to pin B-IA  
const int motorB_B = 7;  // (pwm) pin 7 connected to pin B-IB 
//일괄수정을 쉽게하기위해서 전부 전역으로 선언
//byte speed =255; 

/****** SETUP ******/
void setup()
{
    Serial.begin(9600);
    delay(1000);
    Serial.println("Nrf24L01 Receiver Starting");
    servo1.attach(31); //add servo
    servo2.attach(30);
    radio.begin();
    radio.openReadingPipe(1,pipe);
    radio.startListening();
    pinMode(Red_button,OUTPUT);  
    pinMode(Blue_button,OUTPUT);  
    pinMode(motorA_A, OUTPUT);//4 // set pins to output
    pinMode(motorA_B, OUTPUT);//6
    pinMode(motorB_A, OUTPUT);//5
    pinMode( motorB_B, OUTPUT);//7
    pinMode(motorFliper_A, OUTPUT);//2
    pinMode( motorFliper_B, OUTPUT);//3
    pinMode(motorFliper2_A, OUTPUT);//8
    pinMode( motorFliper2_B, OUTPUT);//9
    
}//--(end setup )---
//
/*  동작을 위한 DC모터 설정.
    굳이 독립함수로 정한이유. 
    -> 조건문에 바로넣으면 수신딜레이가 생김.
    -> 송신기에서 시리얼 출력포함 안시킨이유와 상동.
    nrf24l01은 단순한값을 주고받는것에 최적화되어있다.
    조그만 지연이 동작이상으로 번지기때문에 그것을 미연에 방지하기위해서.*/
 void Fliper_up()
{
         digitalWrite(motorFliper_A,HIGH);
         digitalWrite(motorFliper_B,LOW);
         digitalWrite(motorFliper2_A,HIGH);
         digitalWrite(motorFliper2_B,LOW);
}
 void Fliper_down()
{
         digitalWrite(motorFliper_A,LOW);
         digitalWrite(motorFliper_B,HIGH);
         digitalWrite(motorFliper2_A,LOW);
         digitalWrite(motorFliper2_B,HIGH);
}

 void Fliper_left()
{
         digitalWrite(motorFliper_A,HIGH);
         digitalWrite(motorFliper_B,LOW);
         digitalWrite(motorFliper2_A,LOW);
         digitalWrite(motorFliper2_B,HIGH);
}
 void Fliper_right()
{
         digitalWrite(motorFliper_A,LOW);
         digitalWrite(motorFliper_B,HIGH);
         digitalWrite(motorFliper2_A,HIGH);
         digitalWrite(motorFliper2_B,LOW);
}

void backward()
{
  digitalWrite(motorA_A,LOW);
   digitalWrite(motorA_B,HIGH);
   digitalWrite(motorB_A,LOW);
   digitalWrite(motorB_B,HIGH );
}


void forward()
{
   digitalWrite(motorA_A, HIGH);
   digitalWrite(motorA_B,LOW);
   digitalWrite(motorB_A,HIGH);
   digitalWrite(motorB_B,LOW );
}

void left()
{
   digitalWrite(motorA_A, HIGH);
   digitalWrite(motorA_B,LOW);
   digitalWrite(motorB_A,LOW);
   digitalWrite(motorB_B, HIGH);
}

void right()
{
   digitalWrite(motorA_A, LOW);
   digitalWrite(motorA_B,HIGH);
   digitalWrite(motorB_A,HIGH);
   digitalWrite(motorB_B, LOW);
}
void stop1()
{
   digitalWrite(motorA_A, LOW);
   digitalWrite(motorA_B,LOW);
   digitalWrite(motorB_A,LOW);
   digitalWrite(motorB_B, LOW);
   digitalWrite(motorFliper_A,LOW);
   digitalWrite(motorFliper_B, LOW);
   digitalWrite(motorFliper2_A,LOW);
   digitalWrite(motorFliper2_B, LOW);
}
//동작함수끝

/****** LOOP: RUNS CONSTANTLY ******/
void loop() 
{
  
   if ( radio.available() )
   {
      bool done = false;
       //bool done2 = false;
      while (!done) //좌표값이 이상없이 수신되고있으면 좌표에따라 동작을설정.
      { 
        done = radio.read( joystick, sizeof(joystick) );
    // Read the data payload until we've received everything
      if (joystick[0] >700 && joystick[0] <1024)//x
        {
         forward();  
         delay(10);
        }
        
      if (joystick[0] >0 && joystick[0] <400 ) //x
         {        
         backward();
         delay(10); //delay(10)을 추가하면 DC모터의 노이즈제거에 도움이된다.
         }
      if (joystick[1] >0 && joystick[1] <400)//y
        {
           left(); 
           delay(10);  
      }
      if (joystick[1] <1024 && joystick[1] >700)//y
       {
         right();
         delay(10);
       }
       
        if (joystick[4] >700 && joystick[4] <1024)//y
       {
        Fliper_up();
         delay(10);
       }
      if (joystick[4] >0 && joystick[4] <400)//y
        {
               Fliper_down();
               delay(10);  
      }
      
     if (joystick[5] >0 && joystick[5] <400)//y
     {
           Fliper_left(); 
           delay(10);  
      }
      if (joystick[5] <1024 && joystick[5] >700)//y
       {
          Fliper_right();
         delay(10);
       }
       

      
       if (joystick[0] >400 && joystick[0] <600 && joystick[1] >400 && joystick[1] <599 && joystick[4] >400 && joystick[4] <599 && joystick[5] >400 && joystick[5] <599)
        {
         stop1(); 
         delay(10);
        }
      ////////***********주행관련동작 끝********************/////////////
      
      if(joystick[6] == 0){
       delay(10);
       digitalWrite(Red_button, HIGH);
      }
       else {
       digitalWrite(Red_button, LOW);
     }
     //targeting laser
     if(joystick[7] == 0){
     delay(10);
     digitalWrite(Blue_button, HIGH);
      }
       else {
       digitalWrite(Blue_button, LOW);
     }
     //burning laser
     //점등관련끝
   ////////*******************************/////////////
//조이스틱으로 서보의 좌표를 계산하기위함.
       joystickPanSpeed = (joystick[2] - 512) / 50;
     // -512 to provide equal +/- numbers
     joystickTiltSpeed = (joystick[3] - 512) / -50;
     // negative 50 to reverse direction

     servoPanPosition = constrain((servoPanPosition + joystickPanSpeed), 1, 180);
     servoTiltPosition = constrain((servoTiltPosition + joystickTiltSpeed), 1, 180);
     // constarin function to not exceed servo limits

     servo1.write(servoPanPosition);
     servo2.write(servoTiltPosition);
     //서보관련끝.

          Serial.print("X = ");
          Serial.print(joystick[0]);
          Serial.print(" Y = ");
          Serial.print(joystick[1]);
          Serial.print(" PAN = ");
          Serial.print(joystick[2]);
          Serial.print(" TILT = ");
          Serial.print(joystick[3]);
          Serial.print(" Fliper = ");
          Serial.print(joystick[4]);
          Serial.print(" Fliper2 = ");
          Serial.print(joystick[5]);
          Serial.print(" msg ");
          Serial.print(joystick[6]);
          Serial.print(" msg2 ");
          Serial.println(joystick[7]);
        // delay(500);
        //에러확인용 시리얼값 출력.
   
      }
  }
 
  else 
  { 
       Serial.println("No radio available");//수신받고있지 못하는경우
  }
}//--(end main loop )---

