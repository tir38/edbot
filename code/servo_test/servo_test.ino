// Arduino 4 DOF robotic arm controller
// Jason Atwood
// tir38.com
// 4/25/2012

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

// declare joint variables
int theta[4] = {90, 90, 90, 90}; // current joint angle, degrees. start at timeout pos
int desired[4];           // desired joint angle, degrees
int desired_speed;        // angular joint velocity, degrees per second
int step_size;            // time inbetween steps, milliseconds
int dir;                  // motor direction +1 or -1
int hold = 0;

// ++++++++++++++ as determined by "arduino_reference_position" function ++++++
int reference_position[] = {40, 120, 85, 65};  // joint angles corresponding to reference position, degrees
// ++++++++++++++++++++++++++

int timeout_angles[] = {90, 90, 90, 90}; // joint angles to move to during timeout
 

// Serial related declarations
String inputString;
char inputChar[20]; // 
char delims[] = ",";
char *temp;
int i = 0;
int timeout_counter = 0; // counter to determine when to timeout on serial comm
int timeout_at = 4;

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

  // initialize serial communication
  Serial.begin(19200);
  
  serial_write(theta); // write current joint angles to MATLAB
  delay(4000);
} 

// ======== main loop ===================================================
// read in one line of joint angles from MATLAB
// move to those angles at desired speed
// report to MATLAB that those angles have been reached

void loop() 
{ 
  serial_read();  // read in desired joint angles from MATLAB
  if (timeout_counter < timeout_at) // if not timedout
  {
    delay(100);
    move_servos(desired, theta, desired_speed);  // step servos at desired speed
    serial_write(theta); // write current joint angles to MATLAB
    delay(100);
   }
  else if (timeout_counter >= timeout_at) // if reached timeout
  { 
    time_out(); // enter timeout function
  }
  timeout_counter++; // increment timeout counter
}

// ======== serial read ===================================================
// reads in one line from Matlab, containing 5 values
// for example "90,35,120,35,20\n"
// assigns values to desired joint angles (1-4) and desired speed (5th)
// (remember that int arrays start with index = 0)

void serial_read()
{ 
  while (Serial.available() > 0) // keep reading as long as bytes avaiable
  {
    char incomingByte = (char)Serial.read(); // Read a byte from the serial buffer.
    inputString += incomingByte;             // add current character to string
  }
  if (inputString != "") // if the string is not NULL
  {  
    // parse string
    inputString.toCharArray(inputChar, 20); // convert to char array 
      
    temp = strtok(inputChar, delims);       // tokenize first value
    while (temp != NULL)
    {
      if (i == 4) // if reading in 5th value
      {
        desired_speed = atoi(temp); // assign as desired speed
      }
      else // if reading in 1st through 4th values
      {
        desired[i] = atoi(temp); // assign as desired joint angle
      }
      temp = strtok(NULL, delims); // tokenize next value
      i ++;  // increment counter
      timeout_counter = 0;
    } 
    
    inputString = ""; // reset string
    i = 0 ; // reset counter 
     
  }
}

// ======== serial write ===================================================
// writes current joint angles to Matlab at specified interval of the form
// "90,35,120,35\\n"
// (remember that int arrays start with index = 0)

void serial_write(int theta[])
{
    // uncomment for debugging
    // Serial.println("Arduino says: Telling Matlab current joint angles.");
    Serial.print("[");
    Serial.print(theta[0]);
    Serial.print(",");
    Serial.print(theta[1]);
    Serial.print(",");
    Serial.print(theta[2]);
    Serial.print(",");
    Serial.print(theta[3]);
    Serial.println("]");
//    Serial.println("==========================================");
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
    serial_write(theta);  // write current position to serial  
  }
  
  // uncomment for debugging:
  // Serial.println("Arduino says: Done moving.");
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

// ======== time out ===================================================
// performs the following actions when void loop decides timeout has occured
void time_out ()
{
  Serial.println("==========================================");
  Serial.println("Arduino says: Timeout reached. Moving to timeout position");  
  move_servos(timeout_angles,theta,12); //move to reference position, slowly
  delay(500);
  Serial.println("Arduino says: Closing serial connection. Reset Arduino to reconnect.");
  delay(1000);
  Serial.end();
}
