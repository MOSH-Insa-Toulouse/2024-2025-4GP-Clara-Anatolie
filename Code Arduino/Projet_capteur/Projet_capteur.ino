//Inclusion des bibliothèques:
#include <SoftwareSerial.h>  //Bluetooth
#include <Servo.h>  //Servo

//Definitions:
#define baudrate 9600

#define rxPin 11  //Bluetooth
#define txPin 10

#define encoder0PinA  2 //
#define encoder0PinB  4 

//Autres trucs:
SoftwareSerial mySerial (rxPin, txPin); //Bluetooth

Servo myservo; //Servo
int pos = 0;

int encoder0Pos = 0; //Encodeur

const int flexPin = A0; //flexs

void setup() {
  // Module Bluetooth:
  pinMode (rxPin,INPUT);
  pinMode (txPin,OUTPUT);

  mySerial.begin(baudrate);
  Serial.begin(baudrate);

  // Servomotor:
  myservo.attach(9);

  //Flex Sensor:
  pinMode(flexPin, INPUT);

}

void loop() {
  myservo.write(pos);
  value = analogRead(flexPin);

}
