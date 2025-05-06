/*Reste à faire:
-mettre potentiomètre digital
-décommenter bluetooth et le mettre au bon endroit
-envoyer val graphite et flexs sur tel
-coder servo avec bluetooth
*/

//Inclusion des bibliothèques:
#include <SoftwareSerial.h>  //Bluetooth
#include <Servo.h>  //Servo
#include <Wire.h>   //Encodeur + OLED
#include <Adafruit_SSD1306.h>   //OLED
#include <SPI.h>
#include <stdlib.h>

//Definitions:
#define baudrate 9600

//----------------------------------------------------------------------

  //Bluetooth:
#define rxPin 8
#define txPin 7

  //Encodeur rotatoire:
#define Encodeurclkpin  2  //CLK Output A avec interruption
#define Encodeurdtpin 4  //DT Output B
#define Encodeurswpin 6   //Switch

  //OLED:
#define nombreDePixelsEnLargeur 128
#define nombreDePixelsEnHauteur 64
#define brocheResetOLED -1
#define adresseI2CecranOLED 0x3C
#define nb_item 3

//Autres trucs:
  //Bluetooth:
SoftwareSerial mySerial (rxPin, txPin); 

  //Servo:
Servo myservo;
int Pos_servo = 0;
int vitesse = 0;

  //Encodeur:
volatile int Pos_encodeur = 0;
int etat_bouton = 0; // variable pour stocker la lecture de l'etat du bouton
int Menu = 0; // variable pour stocker la lecture de l'etat du bouton la 2ème fois

  //FlexS:
const int flexPin = A1;
int val_flexs = 0;
float Vflexs, Rflexs = 0.0;
const float VCC = 5.0;
const float Rdiv = 39000.0;
const float flatres = 25000.0;
const float bentres = 100000.0;

  //Capteur graphite:
const int graphitepin = A0 ;
int val_graph = 0;
float VGraph, RGraph = 0.0;
const float R1 = 100000;
const float R3 = 100000;
const float R5 = 10000;


  //OLED:
int choix = 0;

  //DIGITAL POTENTIOMETRE:
#define MCP_NOP                 0b00000000
#define MCP_WRITE               0b00010001
#define MCP_SHTDWN              0b00100001
//#define ssMCPin                 10

const int csPin                = 10;
const int maxPositions         = 256;
const long rAB                 = 33800; //comment on le sait
const byte rWiper              = 125;
float R2;
const byte pot0                = 0x11;
const byte pot0shutdown        = 0x21;

Adafruit_SSD1306 ecranOLED (nombreDePixelsEnLargeur, nombreDePixelsEnHauteur, &Wire, brocheResetOLED);

  //Autres variables:
unsigned long previousMillis = 0;

//---------------------------------------------------------------

//Fonctions:



void setup() {
  //Encodeur rotatoire:
  pinMode(Encodeurclkpin, INPUT); 
  digitalWrite(Encodeurclkpin, HIGH);  // Turn on pullup resistor

  pinMode(Encodeurdtpin, INPUT); 
  digitalWrite(Encodeurdtpin, HIGH);  // Turn on pullup resistor

  pinMode(Encodeurswpin, INPUT); 
  digitalWrite(Encodeurswpin, HIGH);

  attachInterrupt(0, doEncoder, RISING); // On met une interruption sur l'encodeur pin 2 ou 0?

  //Module Bluetooth:
  pinMode (rxPin,INPUT);
  pinMode (txPin,OUTPUT);

  mySerial.begin(baudrate);
  Serial.begin(baudrate);

  //Servomotor:
  myservo.attach(9);

  //Flex Sensor:
  pinMode(flexPin, INPUT);

  //Capt graphite:
  pinMode(graphitepin, INPUT);

  //OLED:
  if (!ecranOLED.begin (SSD1306_SWITCHCAPVCC, adresseI2CecranOLED)) // Arrêt du programme (boucle infinie) si échec d'initialisation
    while(1);

 //Digital Potentiometre:
  pinMode (csPin, OUTPUT); //select pin output
  digitalWrite(csPin, HIGH); //SPI chip disabled
  pinMode (csPin, INPUT);
  SPI.begin();

  ecranOLED.clearDisplay();

  delay(500);

  Calibration();
  //setPotWiper(pot0, 255);

  //Pour indiquer qu'on démarre:
  Serial.println(F("Let's go"));

}

void loop() {

  
  //int instr [100];
  //int i = 0;

  // Serial.println(F("ça marche ou quoi"));
  //Afficher_Menu();

  char str[16];
  float val = graphiteSensor();
  dtostrf(val, 10, 2, str);
  Serial.println(str);
  Serial.println("OK");
  delay(1000);


  /*Serial.println (Pos_encodeur, DEC);  //Angle = (360 / Encoder_Resolution) * encoder0Pos
  if (Pos_encodeur >= 30){
    Pos_encodeur = 0;
  }
  Pos_servo=160/30*Pos_encodeur; 
  myservo.write(Pos_servo);
  */
  
  //char someChar[32]={0};

  /*if (Serial.available()>0){
    instr[i++]=Serial.read();
  }*/

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

//Valeur FlexSensor :
float flexSensor() {
  char valEnvoie[16];
  float ADCflex = analogRead(flexPin);
  float Vflexs = ADCflex * VCC / 1024.0;
  float Rflexs = Rdiv * (VCC / Vflexs - 1.0);
  dtostrf(Rflexs, 16, 0, valEnvoie);
  mySerial.write(valEnvoie);
  delay(1000);
  return Rflexs;
}

//Valeur Graphite sensor : 
float graphiteSensor() {
  float ADCgraph = analogRead(graphitepin); //changement de pin
  float VGraph = ADCgraph * VCC / 1024.0;
  
  RGraph = R1 * (1 + R3/R2) * (VCC / VGraph) - R1 - R5; 

  /*if (VGraph == 0) {
    RGraph = 0.0;
  }*/

  return RGraph;
}

float graphiteSensor_voltage() {
  float ADCgraph = analogRead(graphitepin); //changement de pin
  float VGraph = ADCgraph * VCC / 1024.0; 

  /*if (VGraph == 0) {
    RGraph = 0.0;
  }*/

  return VGraph;
}

//Digital potentiometre
void setPotWiper(int addr, int pos) {
  pos = constrain(pos, 0, 255);
  digitalWrite(csPin, LOW);
  SPI.transfer(addr);
  SPI.transfer(pos);
  digitalWrite(csPin, HIGH);

  R2 = ((rAB * pos) / maxPositions) + rWiper;
}

void Calibration() {
  float target = 3.0, tol = 0.15;
  int pos = 0;
  char chaine[10];

  do {
    setPotWiper(pot0, pos);
    pos += 5;
    delay(200);
  } while ((graphiteSensor_voltage() < (target - tol) || graphiteSensor_voltage() > (target + tol)) && pos <= 265);
  
  dtostrf(pos, 5, 2, chaine);
  
  if (pos < 265) {
    Serial.println(F("Potentiometer calibrated at position : "));
    Serial.print(chaine);

    float val = graphiteSensor();
    dtostrf(val, 10, 2, chaine);
    Serial.println(F("Value : "));
    Serial.print(chaine);
  }
  else {
    Serial.println(F("Potentiometer not calibrated at target 3V"));
    dtostrf(pos, 5, 2, chaine);
    Serial.print(chaine); //à enlever + tard
  }
} 


//Encodeur
void doEncoder() {
  if ( (digitalRead(Encodeurclkpin)==HIGH) && (digitalRead(Encodeurdtpin)==HIGH) ) { 
    Pos_encodeur++;
  } 
  else if ( (digitalRead(Encodeurclkpin)==HIGH) && (digitalRead(Encodeurdtpin)==LOW) ) {  //
    Pos_encodeur--;
  }  
}

void appui_bouton (){
  etat_bouton = digitalRead(Encodeurswpin);
  if (etat_bouton == 0) {
    if (Menu == 0) {
      Menu++;
    }
    else {
      Menu--;
    }
  }
  // Serial.println(etat_bouton, DEC);
  //Serial.println(Menu, DEC);
}

//OLED
void Afficher_Menu (){
  unsigned long currentMillis = millis ();
  float valeur;
  char place[16];


  if (currentMillis - previousMillis >= 500){
    previousMillis = currentMillis;

    appui_bouton();
    choix = Pos_encodeur % nb_item;

    switch (abs(choix)){
      case 0 :
        if (Menu == 0) {
          ecranOLED.clearDisplay();   // Effaçage de l'intégralité du buffer
          ecranOLED.setTextSize(2);   // Taille du texte
          ecranOLED.setCursor(0, 0);
          ecranOLED.setTextColor (SSD1306_WHITE, SSD1306_BLACK);
          ecranOLED.println (F("*MENU*"));
          ecranOLED.setTextSize(1);
          ecranOLED.setTextColor(SSD1306_BLACK, SSD1306_WHITE);
          ecranOLED.println(F("Mesure Flex Sensor"));
          ecranOLED.setTextColor(SSD1306_WHITE, SSD1306_BLACK);
          ecranOLED.println(F("Mesure Graphite Sensor"));
          ecranOLED.println(F("Servomotor"));
          ecranOLED.display();
        }
        else if (Menu == 1) {
          ecranOLED.clearDisplay();   // Effaçage de l'intégralité du buffer
          ecranOLED.setTextSize(2);   // Taille du texte
          ecranOLED.setCursor(0, 0);
          ecranOLED.setTextColor (SSD1306_WHITE, SSD1306_BLACK);
          ecranOLED.println (F("*Menu 1*")); 
          ecranOLED.setTextSize(1);
          ecranOLED.println (F("FlexSensor :"));
          ecranOLED.setTextSize(1);
          valeur = flexSensor();
          dtostrf(valeur, 16, 2, place);
          ecranOLED.println (place);
          ecranOLED.display();
        }
        break;

      case 1:
          if (Menu == 0) {
            ecranOLED.clearDisplay();   // Effaçage de l'intégralité du buffer
            ecranOLED.setTextSize(2);   // Taille du texte
            ecranOLED.setCursor(0, 0);
            ecranOLED.setTextColor (SSD1306_WHITE, SSD1306_BLACK);
            ecranOLED.println (F("*MENU*"));
            ecranOLED.setTextSize(1);
            ecranOLED.setTextColor(SSD1306_WHITE, SSD1306_BLACK);
            ecranOLED.println(F("Mesure Flex Sensor"));
            ecranOLED.setTextColor(SSD1306_BLACK, SSD1306_WHITE);
            ecranOLED.println(F("Mesure Graphite Sensor"));
            ecranOLED.setTextColor(SSD1306_WHITE, SSD1306_BLACK);
            ecranOLED.println(F("Servomotor"));
            ecranOLED.display();
          }

          else if (Menu == 1) {
            ecranOLED.clearDisplay();   // Effaçage de l'intégralité du buffer
            ecranOLED.setTextSize(2);   // Taille du texte
            ecranOLED.setCursor(0, 0);
            ecranOLED.setTextColor (SSD1306_WHITE, SSD1306_BLACK);
            ecranOLED.println (F("*Menu 2*")); 
            ecranOLED.setTextSize(1);
            ecranOLED.println (F("Graphite Sensor :")); 
            ecranOLED.setTextSize(1);
            valeur = graphiteSensor();
            dtostrf(valeur, 16, 2, place);
            ecranOLED.println (place); 
            ecranOLED.display();
          }
        break;

      case 2:
        if (Menu == 0) {
          ecranOLED.clearDisplay();   // Effaçage de l'intégralité du buffer
          ecranOLED.setTextSize(2);   // Taille du texte
          ecranOLED.setCursor(0, 0);
          ecranOLED.setTextColor (SSD1306_WHITE, SSD1306_BLACK);
          ecranOLED.println (F("*MENU*"));
          ecranOLED.setTextSize(1);
          ecranOLED.setTextColor(SSD1306_WHITE, SSD1306_BLACK);
          ecranOLED.println(F("Mesure Flex Sensor"));
          ecranOLED.println(F("Mesure Graphite Sensor"));
          ecranOLED.setTextColor(SSD1306_BLACK, SSD1306_WHITE);
          ecranOLED.println(F("Servomotor"));
          ecranOLED.display();
        }
        else if (Menu == 1){
            Serial.println(F("servo"));   //faire un bouton sur l'appli qui fait bouger le servo à une certaine pos 

        }
        break;
    }
  }
}
