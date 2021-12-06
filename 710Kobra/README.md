/* 

 The program creates an array of "motors"
 structure which describes Motor <BR> 
 Pins, Sensor readings, Expected sensor reading, Time motor started, 
 Time when to stop, Does the motor running.
 
 How it works. 
 
 In Setup() section it stops all motors. <BR>
 In Loop() section it Checks if the target position equals current position <BR>
 If difference is too big it starts a motor for 200 millis. <BR>
 If target position is reached or time is up it stops motor. <BR>
 Target positions for motors could be changed by sending character commands to USB <BR>
 "1","2","3","4","5","6","7","8"  readUsbSetTarget() for molre details <BR>
 Use SerialMonitor to send commands. <BR>
  
 (C)PinAxe 6/17/2018 
           3/13/2019
  
 License - WTFPL
  
*/ 
