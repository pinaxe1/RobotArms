/* The program creates an array of "motors"
 structures which describe Motor pins, Sensor readings, Expected sensor reading, Time motor started, 
 Time when to stop, Does the motor running.

 How it works. 
 In Setup() section it stops all motors.
 In Loop() section it Checks if target pos equals current pos
 If difference is too big it starts a motor for 200 millis.
 If target position is reached or time is up it stops motor.
 Target positions for motors could be changed by sending character commands to USB
 "1","2","3","4","5","6","7","8"  readUsbSetTarget
 Use SerialMonitor to send commands.

 (C)PinAxe 6/17/2018 
           3/13/2019
 License - Code is in Public Domain.
*/ 
# define motors_n 6

typedef struct t_motor 
{   byte pin1;       // Pin1 Pin2 are arduino pins assigned to a motor driver 
    byte pin2;       // To invert motor direction just swap numbers of Pin1 Pin2 in Motor[] array see below
    byte sensorPin;  // Pin assigned to read position sensor (pot)analog value
    int  precision;  // Actually it is a tolerance interval for position sensor. 0 means no tolerance.
                     // any difference (pos1-pos2) bigger then Precision will start corrective actions. See function ifPotsRunaMotor.
    int  pos1;       //  to store current position sensor reading
    int  pos2;       //  to store Target position sensor value
    long int starttime; // When motor started. Actually useless. Gotta remove it.
    long int stoptime;  // When to stop motor.
    bool runs;        //  indicates that a motor is running.
};


t_motor motor[motors_n]={
  { 2, 3,0,4,0,0,0,0,0},
  { 4, 5,2,4,0,0,0,0,0},
  { 7, 6,1,4,0,0,0,0,0},
  { 8, 9,3,4,0,0,0,0,0},
  {10,11,4,4,0,0,0,0,0},
  {12,13,5,4,0,0,0,0,0}
};
  
void setup() 
{  delay(2000);
   Serial.begin(9600);
   for (byte i=0;i<motors_n;i++){ // Init control and sensor lines
      pinMode(motor[i].pin1,     OUTPUT);
      pinMode(motor[i].pin2,     OUTPUT);
      pinMode(motor[i].sensorPin, INPUT);
   }
   readPositionSensors(motor[0]); //
   ResetTargetPosition(motor[0]);    // Set target motor positions equal to a current position so the arm wouldnt move
} 
 
void loop() 
{
  readPositionSensors(motor[0]);
  ifPotsRunaMotor    (motor[0]);
  //ifTimeIsUpStopMotor(motor[0]);  // Useless since ifPotsRunaMotor will start the motor all the time until reached a target
  readUsbSetTarget   (motor[0]);
}; 

// The function starts motors if the difference between a current position (pos1) and target position (pos2)
// is big enough. Stops motors if target reached within precision
void ifPotsRunaMotor(struct t_motor &moto)
{ 
  for (int i=0; i<motors_n ;i++) {
    int dif=motor[i].pos2-motor[i].pos1;
    bool sign=dif>0;
    if (abs(dif)> motor[i].precision) 
         runMotor(motor[i],sign,200);// StartMotor If difference is big enough  
    else stopMotor(motor[i]); // StopMotor if precision is reached. 
  }
};
////////////////////////
// ResetTargetPosition The function sets Target arm position equal to current arm position to initialise a robot 
// and make sure it would not move
void ResetTargetPosition(struct t_motor &moto)
{  for (int i=0; i<motors_n ;i++) {  
     motor[i].pos2=motor[i].pos1;
     Serial.print(motor[i].pos1); Serial.print(' '); 
   } Serial.println( ); 
};

////////////////////////
// readUsbSetTarget The function gets commands from Usb and increments decrements targets motor[i].pos2
//
void readUsbSetTarget(struct t_motor &moto)
{if (Serial.available() > 0)  { // check for incoming serial data:
   char inChar = Serial.read(); // read incoming serial data:
   switch(inChar) {
   case '1' :motor[0].pos2-=10; break; 
   case '2' :motor[0].pos2+=10; break; 
   case '3' :motor[1].pos2-=10; break; 
   case '4' :motor[1].pos2+=10; break; 
   case '5' :motor[2].pos2-=10; break; 
   case '6' :motor[2].pos2+=10; break; 
   case '7' :motor[3].pos2-=10; break; 
   case '8' :motor[3].pos2+=10; break; 
   case '0' :ResetTargetPosition(motor[0]);
   }
 }
 
}

void readPositionSensors(struct t_motor &moto)
{  for (int i=0; i<motors_n ;i++) {
      motor[i].pos1=analogRead(motor[i].sensorPin);
      Serial.print(motor[i].pos1);  
      Serial.print(" ");  
      Serial.print(motor[i].pos2);  
      Serial.print("  ");  
   }
   Serial.println(" ");  
};

bool ifTimeIsUpStopMotor(struct t_motor &moto)
{  unsigned long aTime =millis();
   bool finished = true;
   for (int i=0; i<motors_n ;i++) {
      if ( motor[i].runs && aTime>=motor[i].stoptime )
           stopMotor(motor[i]);
      if (motor[i].runs) finished=false;
      }       
   return(finished);   
};

void runMotor(struct t_motor &motor, bool forward, int time2run) 
{if(!motor.runs){
   digitalWrite( motor.pin1,  forward );
   digitalWrite( motor.pin2, !forward );
   motor.starttime=millis();
   motor.stoptime=motor.starttime+time2run;
   motor.runs=true;
 }
};

void stopMotor(struct t_motor &motor) 
{if(motor.runs){  
   digitalWrite( motor.pin1, 0);
   digitalWrite( motor.pin2, 0);
   motor.starttime=0;
   motor.stoptime=0;
   motor.runs=false;   
 }   
};


