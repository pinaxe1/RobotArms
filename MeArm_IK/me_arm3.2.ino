/*
Derived from Small Hammer SNAM500 4DOF robot arm software
https://smallhammer.cc/product/snarm500/
https://smallhammer.cc/docs/snam500/arduino-code/
Covered by LGPL license
The programm comtrols robot arm in two modes a "Pot mode" and a "PC mode"
In default "Pot mode" it does potentiometer readings from SmallHammer board and sets servo angles accordingly.
Just turn potentiometers and arm will follow.
To switch into PC mode - enter character "n" from SerialMonitor.
then enter commands to control servos. like that.
Command examples:
120a - open the claw  
160a - close the claw 
90b - bends arm with an elbow servo into middle position 90 degrees
90c - sets shoulder servo into middle position 90 degrees
180d - rotates moutn servo to the far right. 180 degrees

The number is angle. The letter is the servo name.
You may enter one commmand at a time or whole string of commands
See a cheat sheet and a Dance() function below.

     140b  80c      - the arm  in a T pose
160a  90b 179c 90d  - closed claw stretched arm on the floor direction strait
     110b  20c      - fully retract arm
*/

#include "VarSpeedServo.h"
#include "ik.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////
int pace =10; // Speed for stepper motors. 0 - Unlimited 1- Lowest ... to Highest -255 
VarSpeedServo servo1, servo2, servo3, servo4;
int potpin1 = 0, potpin2 = 1, potpin3 = 2, potpin4 = 3;
static int val1,val2,val3,val4,valx,valy,valz;
static int v = 0;
static byte stack[3];
byte  mode,index=0;
boolean modep=true;


void setup()  ////////////////////////////////////////////////////////////////////////////////////////
{
  servo1.attach(6);   // Claw pin
  servo2.attach(9);   // Elbow pin
  servo3.attach(10);  // Shoulder pin
  servo4.attach(11);  // Mount pin
  Serial.begin(9600);
}


void loop() //////////////////////////////////////////////////////////////////////////////////////////
{
  if ( Serial.available()){
    char ch = Serial.read();
    ParseCommandsSwitchModes(ch);
  }
  if(mode==0) readPots();
  DoCommands();
}


void ParseCommandsSwitchModes(char ch){ ////////////////////////////////////////////////////////////////////////
  Serial.print(ch);
  switch(ch) {
    case '0'...'9': v=v*10+(ch-'0');    break; // Kinda conterintuitive but it converts char sequence into a Number. I.e. caracter sequence "1" "2" "3" becomes an integer 123.
    case '-':  v=-v; break; //To set coordinate negative use "-" after value"-".  "12-x" vill set x coordinate to -12
    case 'a':  if(mode==1) val1=v; v=0; break;
    case 'b':  if(mode==1) val2=v; v=0; break;
    case 'c':  if(mode==1) val3=v; v=0; break;
    case 'd':  if(mode==1) val4=v; v=0; break;
    case 'x':  if(mode==1) valx=v; v=0; break;
    case 'y':  if(mode==1) valy=v; v=0; break;
    case 'z':  if(mode==1) valz=v; v=0; break;
    case 's':  solveArm(valx,valy,valz); v=0; break;
    case 'S':  goXYZ   (valx,valy,valz); v=0; break;  
    case 'm':  mode=0;  Serial.print("Mode = pot  "); Serial.println(mode); v=0; break;
    case 'n':  mode=1;  Serial.print("Mode = pc   "); Serial.println(mode); v=0; break;
    case 'o':  modep=!modep; Serial.print("Mode = o"); Serial.println(modep); v=0; break;
    case 'p':  Serial.print("Dance "); v=0; Dance(); break;
    case 't':  delay(v*100);v=0; break;
    
    case ' ':  v=0; Serial.println(); break; // skip "sp" do nothing
    default :  ;//Serial.println("");
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
  servo1.write(val1,pace);    //delay(10);
  servo2.write(val2,pace);   // delay(10);  
  servo3.write(val3,pace);   // delay(10);
  servo4.write(val4,pace);   // delay(10);

  if (modep){
     Serial.print(" ") ;
     Serial.print(val1); Serial.print("a ") ;
     Serial.print(val2); Serial.print("b ") ;
     Serial.print(val3); Serial.print("c ") ;
     Serial.print(val4); Serial.println("d "); 
  }
}
void sendCommands( String data){ ///////////////////////////////////////////////////////////////
  for (int i=0; i<data.length();i++) ParseCommandsSwitchModes(data[i]);
  DoCommands();
}

void solveArm( float x,float y,float z){
  float radBase,radShoulder,radElbow;
  if (solve(x, y, z, radBase, radShoulder, radElbow)) {
     Serial.print(" ");
     Serial.print(round(x));
     Serial.print("x ");
     Serial.print(round(y)); 
     Serial.print("y ");
     Serial.print(round(z)); 
     Serial.print("z ");
     Serial.print(round(radElbow*57.2958));
     Serial.print("b ");
     Serial.print(round(radShoulder*57.2958)); 
     Serial.print("c ");
     Serial.print(round(radBase*57.2958)); 
     Serial.print("d");
  }
}

void goXYZ( float x,float y,float z){
  float radBase,radShoulder,radElbow;
  solveArm(x, y, z); // Just to print results out 
  if (solve(x, y, z, radBase, radShoulder, radElbow)) {
    val4=round( radBase    *57.2958);
    val3=round( radShoulder*57.2958);
    val2=round( radElbow   *57.2958);
    DoCommands();
  }
}
 
void Dance(){
  sendCommands("120a 10t");        //drop
  sendCommands("120b 75c 10t");    //Rretract arm
  sendCommands("120b 165c 90d 20t");//go to first station
  sendCommands("160a 10t");       //grab
  sendCommands("75c 20t");       //arm Up
  sendCommands("0d 20t");       //turn to second station
  sendCommands("46b 100c 20t");//go to second station
  sendCommands("120a 10t");   //drop
  sendCommands("120b75c 20t");   //Rretract arm
  sendCommands("0d 20t");   //turn to second station
  sendCommands("46b 100c 20t");//go to second station
  sendCommands("160a 10t");   //grab
  sendCommands("120b75c 10t");   //Rretract arm
  sendCommands("120b 165c 90d 20t");//go to first station
  sendCommands("120a 10t");        // drop
  sendCommands("120b75c 10t");    //Rretract arm
}  
