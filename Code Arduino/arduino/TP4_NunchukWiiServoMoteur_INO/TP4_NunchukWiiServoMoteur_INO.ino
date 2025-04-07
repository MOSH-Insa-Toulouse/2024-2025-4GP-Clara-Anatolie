#include <math.h>
#include <Servo.h> 
#include "Wire.h"
#include "WiiChuck.h"

Servo myservo;  
Servo myservo1; // create servo object to control a servo

WiiChuck chuck = WiiChuck();
int angleStart, currentAngle, currentAngle1;
int tillerStart = 0;
double angle;

void setup()
{
  //nunchuck_init();
  Serial.begin(115200);
  chuck.begin();
  chuck.update();
  //chuck.calibrateJoy();
  myservo.attach(9); 
  myservo1.attach(8); // attaches the servo on pin 9 to the servo object 
}


void loop()
{
  delay(20);
  chuck.update(); 
  
  currentAngle = chuck.readPitch();
  currentAngle1 = chuck.readRoll();
  constrain(currentAngle, 10, 130);
  currentAngle = map(currentAngle, 10, 130, 0, 180);
  constrain(currentAngle1, 10, 130);
  currentAngle1 = map(currentAngle1, 10, 130, 0, 180);
  
  myservo.write(currentAngle);
  myservo1.write(currentAngle1);  // sets the servo position according to the scaled value 
  
  Serial.print(", Pitch = ");
  Serial.print(chuck.readPitch());
  Serial.print(", Roll = ");
  Serial.print(chuck.readRoll());
  Serial.println();
  
}

