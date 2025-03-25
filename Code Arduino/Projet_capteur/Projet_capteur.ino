//Inclusion des bibliothèques:
#include <SoftwareSerial.h>  //Bluetooth
#include <Servo.h>  //Servo

//Definitions:
#define baudrate 9600

  //Bluetooth:
#define rxPin 11
#define txPin 10

  //Encodeur rotatoire:
#define Encodeurclk  2  //CLK Output A avec interruption
#define Encodeurdt  3  //DT Output B
#define Encodeursw 6   //Switch

//Autres trucs:
  //Bluetooth:
SoftwareSerial mySerial (rxPin, txPin); 

  //Servo:
Servo myservo;
int Pos_servo = 0;

  //Encodeur:
int Pos_encodeur = 0; 

  //FlexS:
const int flexPin = A0;

//Fonctions:

void doEncoder() {
  if ( (digitalRead(Encodeurclk)==HIGH) && (digitalRead(Encodeurdt)==HIGH)) { 
    Pos_encodeur++;
  } else if ( (digitalRead(Encodeurclk)==HIGH) && (digitalRead(Encodeurdt)==LOW)) {  //
    Pos_encodeur--;
  }
  // Serial.println (encoder0Pos, DEC);  //Angle = (360 / Encoder_Resolution) * encoder0Pos
}

void setup() {
  //Encodeur rotatoire:
  pinMode(Encodeurclk, INPUT); 
  digitalWrite(Encodeurclk, HIGH);  // turn on pullup resistor

  pinMode(Encodeurdt, INPUT); 
  digitalWrite(Encodeurdt, HIGH);

  attachInterrupt(0, doEncoder, RISING); // On met une interruption sur l'encodeur pin 2

  // Module Bluetooth:
  pinMode (rxPin,INPUT);
  pinMode (txPin,OUTPUT);

  mySerial.begin(baudrate);
  Serial.begin(baudrate);

  // Servomotor:
  myservo.attach(9);

  //Flex Sensor:
  pinMode(flexPin, INPUT);

  //Pour indiquer qu'on démarre:
  Serial.println("Let's go\n");

}

void loop() {
  //value = analogRead(flexPin);
  Serial.println (Pos_encodeur, DEC);  //Angle = (360 / Encoder_Resolution) * encoder0Pos
  if (Pos_encodeur >= 30){
    Pos_encodeur = 0;
  }
  Pos_servo=160/30*Pos_encodeur; 
  myservo.write(Pos_servo);
}
