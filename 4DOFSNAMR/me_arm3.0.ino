/*
Derived from Small Hammer SNAM500 4DOF robot arm software
https://smallhammer.cc/product/snarm500/
https://smallhammer.cc/docs/snam500/arduino-code/
Covered by LGPL license
*/

#include "VarSpeedServo.h"
int pace =10; // Speed for stepper motors. 0 - Unlimited 1- Lowest ... to Highest -255 
VarSpeedServo servo1;
VarSpeedServo servo2;
VarSpeedServo servo3;
VarSpeedServo servo4;

int potpin1 = 0;//定义爪舵机电位器为A0口
int potpin2 = 1;//定义小臂舵机电位器为A1口
int potpin3 = 2;//定义大臂舵机电位器为A2口
int potpin4 = 3;//定义转盘舵机电位器为A3口


int val1;
int val2;
int val3;
int val4;

byte  mode ;
boolean modep=true;

void setup()  ////////////////////////////////////////////////////////////////////////////////////////
{

  servo1.attach(11);  servo1.write(140);
  servo2.attach(10);  servo2.write(140);
  servo3.attach(9);   servo3.write(140);            
  servo4.attach(6);   servo4.write(140); 
  


  Serial.begin(9600);
}

void loop() //////////////////////////////////////////////////////////////////////////////////////////
{
  static int v = 0;
  static int s1 = 0;
  static int s2 = 0;
  static int s3 = 0;
  static int s4 = 0;

  if ( Serial.available()) {
    char ch = Serial.read();
    Serial.print(ch);
    switch(ch) {
      case '1'...'9':  v =(ch-'0')*20;    break;
      case 'a':  if(mode==1) servo1.write(v,pace); break;
      case 'b':  if(mode==1) servo2.write(v,pace); break;
      case 'c':  if(mode==1) servo3.write(v,pace); break;
      case 'd':  if(mode==1) servo4.write(v,pace); break;
      case 'm':  mode=0;  Serial.print("Mode = pot  "); Serial.println(mode); break;
      case 'n':  mode=1;  Serial.print("Mode = pc   "); Serial.println(mode); break;
      case 'o':  modep=!modep; Serial.print("Mode = o"); Serial.println(modep); break;
      
      default :  Serial.println("-illegal. Use 1..9 a b c d m n o");

    }
  }

  if(mode==0)  {
    /////爪舵机控制///////
    val1 = analogRead(potpin1);
    val1 = map (val1, 0, 1023, 122, 160);
    servo1.write(val1,pace);
    delay(10);


    //////小臂舵机控制///////
    val2 = analogRead(potpin2);
    val2 = map (val2, 0, 1023, 0, 179);
    servo2.write(val2,pace);
    delay(10);  

    //////大臂舵机控制///////
    val3 = analogRead(potpin3);
    val3 = map (val3, 0, 1023, 40, 179);
    servo3.write(val3,pace);
    delay(10);

    //////爪舵机控制/////// 
    val4 = analogRead(potpin4);
    val4 = map (val4, 0, 1023, 0, 180);
    servo4.write(val4,pace);
    delay(10);
    if (modep){
     Serial.print(val1); Serial.print("a ") ;
     Serial.print(val2); Serial.print("b ") ;
     Serial.print(val3); Serial.print("c ") ;
     Serial.print(val4); Serial.println("d "); 
   }
  }
}

/*
  /------- "a" claw "6a" claw open "8a" claw closed
 |  /----- "b" elbow "2b" elbow folded "7b" unfolded but depends on "c" position  
 | |  /--- "c" shoulde "2c" shoulder folded "9c" unfolded
 | | |  /- "d" mount rotation "1d" right position "9d" left position.
8a7b6c7d
8a7b6c7d
8a7b6c7d
8a7b2c7d - closed folded 
6a7b6c7d - opnen T
8a7b6c7d - close T

*/

