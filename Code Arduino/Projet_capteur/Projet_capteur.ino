//Inclusion des bibliothèques:
#include <SoftwareSerial.h>  //Bluetooth
#include <Servo.h>  //Servo
#include <Wire.h>   //Encodeur + OLED
#include <Adafruit_SSD1306.h>
#include <SPI.h>

//Definitions:
#define baudrate 9600

  //Bluetooth:
#define rxPin 11
#define txPin 10

  //Encodeur rotatoire:
#define Encodeurclk  2  //CLK Output A avec interruption
#define Encodeurdt  3  //DT Output B
#define Encodeursw 6   //Switch

  //OLED:
#define nombreDePixelsEnLargeur 128
#define nombreDePixelsEnHauteur 64
#define brocheResetOLED -1
#define adresseI2CecranOLED 0x3C

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

  //OLED:
int Position = 1;
String Item1 = "Mesure avec Flex Sensor";
String Item2 = "Mesure avec Capteur Graphite";
String Item3 = "Servomotor";

Adafruit_SSD1306 ecranOLED (nombreDePixelsEnLargeur, nombreDePixelsEnHauteur, &Wire, brocheResetOLED);

//Fonctions:

void doEncoder() {
  char * str_Pos_encodeur;
  if ( (digitalRead(Encodeurclk)==HIGH) && (digitalRead(Encodeurdt)==HIGH)) { 
    Pos_encodeur++;
  } else if ( (digitalRead(Encodeurclk)==HIGH) && (digitalRead(Encodeurdt)==LOW)) {  //
    Pos_encodeur--;
  }
  Serial.println(Pos_encodeur, DEC);  //Angle = (360 / Encoder_Resolution) * encoder0Pos
}

void setup() {
  //Encodeur rotatoire:
  pinMode(Encodeurclk, INPUT); 
  digitalWrite(Encodeurclk, HIGH);  // turn on pullup resistor

  pinMode(Encodeurdt, INPUT); 
  digitalWrite(Encodeurdt, HIGH);

  attachInterrupt(0, doEncoder, RISING); // On met une interruption sur l'encodeur pin 2

  //Module Bluetooth:
  pinMode (rxPin,INPUT);
  pinMode (txPin,OUTPUT);

  mySerial.begin(baudrate);
  Serial.begin(baudrate);

  //Servomotor:
  myservo.attach(9);

  //Flex Sensor:
  pinMode(flexPin, INPUT);

  //OLED:
  if (!ecranOLED.begin (SSD1306_SWITCHCAPVCC, adresseI2CecranOLED)) // Arrêt du programme (boucle infinie) si échec d'initialisation
    while(1);

  //Pour indiquer qu'on démarre:
  Serial.println(F("Let's go\n"));

}

void loop() {
  int instr [100];
  int i = 0;
  //int val_flexs =0;

  //val_flexs = analogRead(flexPin);

  ecranOLED.clearDisplay();
  ecranOLED.setTextSize(1);
  ecranOLED.setCursor(0, 0);
  ecranOLED.setTextColor (SSD1306_BLACK, SSD1306_WHITE);
  ecranOLED.println (F("Hello"));
  ecranOLED.display();
  Serial.println(F("ça marche ou quoi"));
  delay(5000);

  /*Serial.println (Pos_encodeur, DEC);  //Angle = (360 / Encoder_Resolution) * encoder0Pos
  if (Pos_encodeur >= 30){
    Pos_encodeur = 0;
  }
  Pos_servo=160/30*Pos_encodeur; 
  myservo.write(Pos_servo);
  */
  
  //char someChar[32]={0};

  if (Serial.available()>0){
    instr[i++]=Serial.read();
  }

  /*while (Serial.available()){
    do{
      someChar[i++] = Serial.read();
    delay(3);
    }while (Serial.available()>0);

    mySerial.println(someChar);
    Serial.println(someChar);
  }
  while (mySerial.available()){
    Serial.print((char)mySerial.read());
  }  */

}
