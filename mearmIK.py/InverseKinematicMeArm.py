# -*- coding: utf-8 -*-
"""
Created on Mon Apr 22 19:58:24 2019
Source: http://appliedgo.net/roboticarm   © 2016-2019 Christoph Berger
@author: Pinaxe
"""


# Only the plain math package is needed for the formulas. 
import math
def car2polar(a,b):
    r = math.sqrt(a*a + b*b)
    theta = math.atan2(b,a)
    return r,theta
# The law of cosines, transfomred so that C is the unknown. The names of the sides and angles correspond to the standard names in mathematical writing. Later, we have to map the sides and angles from our scenario to a, b, c, and C, respectively. 
def lawOfCosines(a, b, c ): # where a,b ajacent c opposite
	return math.acos((a*a + b*b - c*c) / (2 * a * b))
   #return (a*a + b*b - c*c) / (2 * a * b)
# Calculating the two joint angles for given x and y. 

def norm(angle): # normalize angles to 0-Pi range
   if angle<0       : return angle 
   if angle>math.pi : return 2*math.pi-angle
   return angle

def Solve2Angles(x,y,z) :
    # The lengths of the two segments of the robot’s arm. Using the same length for both segments allows the robot to reach the (0,0) coordinate. 
    len1 = 80.0
    len2 = 80.0
    len3 =  0.0
    # First, get the distance and angle from (0,0) to (x,y) 
    R,A0 = car2polar(x , y)
    R -=len3 # An account for a claw length an mountpoint displacement
    dist,D1 =car2polar(R,z) # Then distance and angle to reach z elevation
    D2 = lawOfCosines(dist, len1, len2) #D2 where a = dist, b = len1, are adjacent c = len2 opposite. 
    A1 = D1+D2 # Then A1 is simply the sum of D1 and D2. 
    A2 = lawOfCosines(len1, len2, dist)     # A2 law of cosine, with a = len1, b = len2 adjacent and c = dist opposite.  
    
    # To translate into MeArm angles
    A2 = norm(A2       + A1 );
    A1 = norm(math.pi  - A1 );
    A0 = norm(math.pi/2+ A0 );
    return A0, A1, A2

   
   
def deg(rads) : # Convert radians into degrees. 
   a=180/math.pi*rads
   return a


def main() :

    y =   50.0
    z =   0.0
    for x in range(10,155,5) :
        a0, a1, a2 = Solve2Angles(x, y,z)
        #print(a0,a1,a2)
        print(" %6.2f %6.2f %6.2f " % ( deg(a0), deg(a1), deg(a2)))

	
    
if __name__ == "__main__":
   main()    