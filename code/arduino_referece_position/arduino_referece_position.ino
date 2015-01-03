// Arduino 4 DOF robotic arm reference position setup
// Jason Atwood
// tir38.com
// 4/25/2012
// moves to reference position
// and blinks LED when finished

// ======== declare variables and include libraries ===============================
// libraries
#include <Servo.h> 
#include <string.h>

// declare servo objects
Servo s1;   
Servo s2;
Servo s3;
Servo s4;
Servo s5;
Servo s6;

int ledPin = 13;                 // LED connected to digital pin 13
int theta[] = {0,0,0,0};
int desired_speed = 6;
int step_size;          // time inbetween steps, milliseconds
int dir;                // motor direction +1 or -1
int hold = 0;
  

// ======== setup ===================================================
void setup() 
{ 
  // initialize ports
  s1.attach(2);  
  s2.attach(3);
  s3.attach(4);
  s4.attach(5);
  s5.attach(6);
  s6.attach(7);

  pinMode(ledPin, OUTPUT);      // sets the digital pin as output

  // move servos to "0" position
  s1.write(0);
  s2.write(0);
  s3.write(180);
  s4.write(0);
  s5.write(180);
  s6.write(0);
  delay(500);


// +++++++++++++++++++++++++++++++++++++++++++
// THIS IS THE ONLY SECTION TO CHANGE
// TO DETERMINE WHAT MOTOR ANGLES CORRESPOND TO THE REFERENCE POSITION

  // move servos to reference position
  int reference_position[] = {40, 120, 85, 65};

// +++++++++++++++++++++++++++++++++++++++++++++++ 
  
  move_servos(reference_position, theta, desired_speed);
  delay(100);
} 

// ======== main ===================================================
void loop()
{
  digitalWrite(ledPin, HIGH);   // sets the LED on
  delay(500);                  // waits for a second
  digitalWrite(ledPin, LOW);    // sets the LED off
  delay(500);                  // waits for a second
}

// ======== move servos ===================================================
// takes in desired joint angles, current joint angles, and desired spee
// and moves to those angles at desired speed
// (remember that int arrays start with index = 0)
// uses "hold" to know if entire system is at deiserd position
// each motor that is where it is supposed to be adds 1 to hold
// if hold = 6 then all servos are at correct position and function returns up
// creates local variables of global variables:
// d = desired []
// t = theta []
// d_speed = desired_speed

// hold is local variable
// step size is local variable
// servo opjects are global variables


void move_servos(int d[], int t[], int d_speed)
{
  // compute step size  
  step_size = 1000/d_speed; // milliseconds
  // start moving
  // while at least one angle is not at desired angle stay in this loop
  while (hold < 6 ) // iterate one step at a time
  {
    hold = 0; // reset hold
    
    // -------- 1st servo --------------------------------
    if (d[0] != t[0]) // if not at desired position
    {
      t[0] = t[0] + direction_set(d[0], t[0]); // update angle
      s1.write(t[0]); // move to new angle
    }
    else
    {
      hold++; // adds one to hold
    } 
  
      // -------- 2nd and 3rd servos --------------------------------
    if (d[1] != t[1]) // if not at desired position
    {
      t[1] = t[1] + direction_set(d[1], t[1]); // update angle
      s2.write(t[1]); // move to new angle
      s3.write(flip_flop(t[1])); // move to new angle
    }
    else
    {
      hold = hold +2; // adds two to hold
    } 
  
    // -------- 4th and 5th servos --------------------------------
    if (d[2] != t[2]) // if not at desired position
    {
      t[2] = t[2] + direction_set(d[2], t[2]); // update angle
      s4.write(t[2]); // move to new angle
      s5.write(flip_flop(t[2])); // move to new angle
    }
    else
    {
      hold = hold + 2; // adds one to hold
    }
  
   // -------- 6th servo --------------------------------
    if (d[3] != t[3]) // if not at desired position
    {
      t[3] = t[3] + direction_set(d[3], t[3]); // update angle
      s6.write(t[3]); // move to new angle
    }
    else
    {
      hold++; // adds one to hold
    } 
    // --------------------------------------------------------
    
    delay(step_size);     // wait to iterate
    theta == t;           // update global variable
    
    //uncomment for debugging: 
    //serial_write(theta);  // write current position to serial  
  }
  Serial.println("Arduino says: Done moving.");
  hold = 0; // reset hold
}

// ======== direction determination ===================================================
// takes desired joint angle and current joint angle
// determines if motor needs to move "up" or "down" in direction
// and returns either +1 or -1

int direction_set(int desired_angle, int current_angle)
{
  dir = 0; // reset value
  
  if (desired_angle - current_angle > 0)
  {
    dir = 1;
  }
  else if (desired_angle - current_angle <0)
  {
    dir = -1;
  }
  else
  {
    dir = 0;
  }
  return dir;
}

// ======== flip flop ===================================================
// takes in current joint angle 
// determines motor angle for flipped motors (3 and 5)
// corresponding to angles sent to parallel motors (2 and 4, respectively)
// returns new motor angle
int flip_flop(int input_angle)
{
  int output_angle = 180-input_angle;
  return output_angle;
}


