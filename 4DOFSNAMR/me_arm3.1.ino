/*
Derived from Small Hammer SNAM500 4DOF robot arm software
https://smallhammer.cc/product/snarm500/
https://smallhammer.cc/docs/snam500/arduino-code/
Covered by LGPL license
The programm comtrols robot arm in two modes "Pot mode" and "PC mode"
In default "Pot mode" it makes potentiometer readings from SmallHammer board and sets servo positions accordingly.
So turn potentiometers and arm will follow.
To switch into PC mode just enter character "n" from SerialMonitor.
then enter commands to control servos. like that.
Command examples:
90b - bends arm in elbow servo into middle position
90c - sets shoulder servo into middle position
180d - rotates moutn servo to the far right.
The number is angle. The letter is the servo name.

You may enter one commmand at a time or whole string of commands
See a cheat sheet below.

120a claw open 
160a claw closed
 |  /----- "b" elbow "40b" elbow folded "140b" unfolded but depends on "c" position  
 | |  /--- "c" shoulde "40c" shoulder folded "180c" unfolded
 | | |  /- "d" mount direction "0d" right position "180d" left position. ""
8a1b1c0d - closed claw stretched arm in to right direction
8a7b6c9d - closed claw in T pose direction to the left
     140b  80c     - T pose
160a  90b 179c 90d - closed claw stretched arm on the floor direction strait
     110b  20c      - fully retracted arm
132a 108b 22c 77d     
*/

#include "VarSpeedServo.h"
/////////////////////////////////////////////////////////////////////////////////////////////////////
int pace =60; // Speed for stepper motors. 0 - Unlimited 1- Lowest ... to Highest -255 
VarSpeedServo servo1, servo2, servo3, servo4;
int potpin1 = 0, potpin2 = 1, potpin3 = 2, potpin4 = 3;
static int val1,val2,val3,val4;
static int v = 0;
static byte stack[3];
byte  mode,index=0;
boolean modep=true;


void setup()  ////////////////////////////////////////////////////////////////////////////////////////
{
  servo1.attach(11); 
  servo2.attach(10); 
  servo3.attach(9);  
  servo4.attach(6);  
  Serial.begin(9600);
}


void loop() //////////////////////////////////////////////////////////////////////////////////////////
{
  if ( Serial.available()){
    char ch = Serial.read();
    readComSwitchModes(ch);
  }
  if(mode==0) readPots();
  DoCommands();
}


void readComSwitchModes(char ch){ ////////////////////////////////////////////////////////////////////////
  Serial.print(ch);
  switch(ch) {
    case '0'...'9': v=v*10+(ch-'0');    break; // Kind of conterintuitive but read number characters and converts them into Number. "1" "2" "3" became integer 123.
    case 'a':  if(mode==1) val1=v; v=0; break;
    case 'b':  if(mode==1) val2=v; v=0; break;
    case 'c':  if(mode==1) val3=v; v=0; break;
    case 'd':  if(mode==1) val4=v; v=0; break;
    case 'm':  mode=0;  Serial.print("Mode = pot  "); Serial.println(mode); v=0; break;
    case 'n':  mode=1;  Serial.print("Mode = pc   "); Serial.println(mode); v=0; break;
    case 'o':  modep=!modep; Serial.print("Mode = o"); Serial.println(modep); v=0; break;
    case 'p':  Serial.print("Dance "); v=0; Dance(); break;
    case ' ':  v=0; Serial.println(); break; // skip "sp" do nothing
    default :  Serial.println("-illegal. Use 1..9 a b c d m n o");
  }
}

void readPots(){////////////////////////////////////////////////////////////////////////
    /////爪舵机控制///////
    val1 = analogRead(potpin1);
    val1 = map (val1, 0, 1023, 122, 160);
    //////小臂舵机控制///////
    val2 = analogRead(potpin2);
    val2 = map (val2, 0, 1023, 0, 179);
    //////大臂舵机控制///////
    val3 = analogRead(potpin3);
    val3 = map (val3, 0, 1023, 20, 179);
    //////爪舵机控制/////// 
    val4 = analogRead(potpin4);
    val4 = map (val4, 0, 1023, 0, 179);
}


void DoCommands(){//////////////////////////////////////////////////////////////////////////////
  servo1.write(val1,pace);    delay(10);
  servo2.write(val2,pace);    delay(10);  
  servo3.write(val3,pace);    delay(10);
  servo4.write(val4,pace);    delay(10);

  if (modep){
     Serial.print(" ") ;
     Serial.print(val1); Serial.print("a ") ;
     Serial.print(val2); Serial.print("b ") ;
     Serial.print(val3); Serial.print("c ") ;
     Serial.print(val4); Serial.println("d "); 
  }
}
/*
  120a 76b 171c 78d   go to first station
  160a                pick
  77b 75c             Rretract arm
  46b 155c 0d         go to second place
  120a                drop
  75c                 Rretract arm

 */
void sendCommands( String data){
  for (int i=0; i<data.length();i++) readComSwitchModes(data[i]);
  DoCommands();
}

void Dance(){
  sendCommands("120a ");delay(1000);        // drop
  sendCommands("77b 75c ");delay(1000);       // retract
  sendCommands("120a 76b 171c 78d ");delay(2000);//go to first station
  sendCommands("160a ");delay(1000);          //grab
  sendCommands("77b 75c ");delay(2000);      // retract
  sendCommands("0d ");delay(2000);          //turn to second station
  sendCommands("46b 155c 0d ");delay(2000);//down to second station
  sendCommands("120a ");delay(1000);      //drop
  sendCommands("75c ");delay(1000);      //Rretract arm
  sendCommands("46b 155c 0d ");delay(2000);//go to second station
  sendCommands("160a ");delay(1000);      //grab
  sendCommands("75c ");delay(1000);      //Rretract arm
  sendCommands("76b 171c 78d ");delay(2000);//go to first station
  sendCommands("120a ");delay(1000);       // drop
  sendCommands("77b 75c ");delay(1000);   //Rretract arm
}

/*
Some commands
160a5t80a5t160a  click da beak
40b 90c 2t

35b 96c  2t
44b 100c 2t
56b 110c 2t
68b 120c 2t
80b 130c 2t
90b 140c 2t
104b 150c 2t
113b 160c 2t

110b105b100c44b79b130c120b160c110b115b112b130b170c165c170c175c160c170c90c44b40b44b100c
50b45b43b42b40b44b58b110c70b120c79b130c90b140c105b150c112b160c90c
40b70b120c80b130c112b160c90c40b90b70b90b70c60c50c44b90b110c54b56b90b100c44b42b90b33b95c
*/
