
Derived from Small Hammer SNAM500 4DOF robot arm software <br>
https://smallhammer.cc/product/snarm500/<br>
https://smallhammer.cc/docs/snam500/arduino-code/<br>
Covered by LGPL license<br>
The programm comtrols robot arm in two modes a "Pot mode" and a "PC mode"<br>
In default "Pot mode" it does potentiometer readings from SmallHammer board and sets servo angles accordingly.<br>
Just turn potentiometers and arm will follow.<br>
To switch into PC mode - enter character "n" from SerialMonitor.<br>
then enter commands to control servos. like that.<br>
Command examples:<br>
120a - open the claw <br> 
160a - close the claw <br>
90b - bends arm with an elbow servo into middle position 90 degrees<br>
90c - sets shoulder servo into middle position 90 degrees<br>
180d - rotates moutn servo to the far right. 180 degrees<br>
The number is angle. The letter is the servo name.<br>
You may enter one commmand at a time or whole string of commands<br>
See a cheat sheet and a Dance() function below.<br>
     140b  80c      - the arm  in a T pose<br>
160a  90b 179c 90d  - closed claw stretched arm on the floor direction strait<br>
     110b  20c      - fully retract arm<br>

