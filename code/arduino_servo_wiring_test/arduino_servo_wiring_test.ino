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
  s2.write(90);
  s3.write(90);
  s4.write(90);
  s5.write(90);
  s6.write(90);
  delay(1000);
}

// ======== main ===================================================
void loop()
{
  digitalWrite(ledPin, HIGH);   // sets the LED on
  delay(500);                  // waits for a second
  digitalWrite(ledPin, LOW);    // sets the LED off
  delay(500);                  // waits for a second
}
