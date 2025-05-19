//----------------------------Inclusion des bibliothèques--------------------------

#include <SoftwareSerial.h>  //Bluetooth
#include <Servo.h>  //Servomoteur
#include <Wire.h>   //OLED
#include <Adafruit_SSD1306.h>   //OLED
#include <SPI.h>  //Potentiomètre
#include <stdlib.h>

//----------------------------Définitions et constantes------------------------------

  //Bluetooth:
#define rxPin 8
#define txPin 7

  //Encodeur rotatoire:
#define Encodeurclkpin  2  //CLK Output A, avec interruption
#define Encodeurdtpin 4  //DT Output B
#define Encodeurswpin 6   //Switch

  //OLED:
#define nombreDePixelsEnLargeur 128 //Largeur de l'écran OLED, en pixels
#define nombreDePixelsEnHauteur 64  //Hauteur de l'écran OLED, en pixels
#define brocheResetOLED -1  //Reset de l'écran OLED
#define adresseI2CecranOLED 0x3C
#define nb_item 3 //Nombre de choix dans le menu

  //Servomoteur:
#define servopin 9  // pin contrôlant le moteur (pin PWM)

  //Potentiomètre digital:
#define MCP_NOP                 0b00000000
#define MCP_WRITE               0b00010001
#define MCP_SHTDWN              0b00100001

  //Flex sensor:
#define flexPin A1

  //Capteur graphite:
#define graphitepin A0

  //Autres:
#define baudrate 9600

//----------------------------Variables------------------------------------------

  //Bluetooth:
SoftwareSerial mySerial (rxPin, txPin);
//int i=1; 

  //Servomoteur:
Servo myservo;
int Pos_servo = 0;

  //Encodeur:
volatile int Pos_encodeur = 0;
int etat_bouton = 0; // variable pour stocker la lecture de l'etat du bouton
int Menu = 0;

  //Flex sensor:
float val_flexs = 0;
float Vflexs, Rflexs = 0.0;
const float VCC = 5.0;
const float Rdiv = 39000.0;
const float flatres = 25000.0;
const float bentres = 100000.0;

  //Capteur graphite:
unsigned long previousMillis = 0; //Pour conserver le temps passé depuis le dernier appel de millis()
int val_graph = 0;
float VGraph, RGraph = 0.0;
const float R1 = 100000;
const float R3 = 100000;
const float R5 = 10000;

  //OLED:
int choix = 0;
float lastval = -100.0;

  //Potentiomètre digital:
const int csPin = 10;
const int maxPositions = 256;
const long rAB = 47000;
const byte rWiper = 125;
const byte pot0 = 0x11;
const byte pot0shutdown = 0x21;
float R2;

Adafruit_SSD1306 ecranOLED (nombreDePixelsEnLargeur, nombreDePixelsEnHauteur, &Wire, brocheResetOLED);

//----------------------------Setup------------------------------------------

void setup() {

  //Encodeur rotatoire:
  pinMode(Encodeurclkpin, INPUT); 
  digitalWrite(Encodeurclkpin, HIGH);  // Turn on pullup resistor

  pinMode(Encodeurdtpin, INPUT); 
  digitalWrite(Encodeurdtpin, HIGH);  // Turn on pullup resistor

  pinMode(Encodeurswpin, INPUT); 
  digitalWrite(Encodeurswpin, HIGH);  // Turn on pullup resistor

  attachInterrupt(0, doEncoder, RISING); // On met une interruption sur l'encodeur

  //Module Bluetooth:
  pinMode (rxPin,INPUT);
  pinMode (txPin,OUTPUT);

  mySerial.begin(baudrate);
  Serial.begin(9600);

  //Servomoteur:
  myservo.attach(servopin);

  //Flex Sensor:
  pinMode(flexPin, INPUT);

  //Capt graphite:
  //pinMode(graphitepin, INPUT);

  //OLED:
  if (!ecranOLED.begin (SSD1306_SWITCHCAPVCC, adresseI2CecranOLED)) // Arrêt du programme (boucle infinie) si échec d'initialisation
    while(1);

  ecranOLED.clearDisplay();

  //Digital Potentiometre:
  pinMode (csPin, OUTPUT); //select pin output
  digitalWrite(csPin, HIGH); //SPI chip disabled
  pinMode (csPin, INPUT);
  SPI.begin();

  //Calibration();
  setPotWiper(pot0, 0);

  delay(500);

  //Pour indiquer qu'on démarre:
  Serial.println(F("Let's go"));

}

//----------------------------Loop------------------------------------------

void loop() {

  Afficher_Menu();
  //Serial.println("OK");
  
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

//----------------------------Fonctions------------------------------------------

// Renvoie la valeur de la résisatnce aux bornes du flex sensor

float flexSensor() {
  char val_envoi[20];

  float ADCflex = analogRead(flexPin);
  Vflexs = ADCflex * VCC / 1024.0;
  Rflexs = Rdiv * (VCC / Vflexs - 1.0);

  dtostrf(Rflexs, sizeof(val_envoi)-1, 0, val_envoi);
  mySerial.write(val_envoi);
  delay(500);
  return Rflexs;
}

// Renvoie la valeur de la résisatnce aux bornes du capteur graphite

float graphiteSensor() {
  float ADCgraph = analogRead(graphitepin); //changement de pin
  float VGraph = ADCgraph * VCC / 1024.0;
  
  RGraph = Rdiv*(VCC/VGraph-1); 
  return RGraph;
}

// Renvoie la valeur de la tension aux bornes du capteur graphite

float graphiteSensor_voltage() {
  float ADCgraph = analogRead(graphitepin); //changement de pin
  float VGraph = ADCgraph * VCC / 1024.0; 

  return VGraph;
}

// Règle la résistance de sortie du potentiomètre digital

void setPotWiper(int addr, int pos) {
  pos = constrain(pos, 0, 255);
  digitalWrite(csPin, LOW);
  SPI.transfer(addr);
  SPI.transfer(pos);
  digitalWrite(csPin, HIGH);

  R2 = ((rAB * pos) / maxPositions) + rWiper;
}

// Calibre le potentiomètre digital pour avoir une tensoin de 3V avec une tolérance de 0.15V

void Calibration() {
  float target = 3.0, tol = 0.15;
  int pos = 0;
  char chaine[10];

  do {
    setPotWiper(pot0, pos);
    pos += 5;
    delay(200);
  } while ((graphiteSensor_voltage() < (target - tol) || graphiteSensor_voltage() > (target + tol)) && pos <= 265);
  
  dtostrf(pos, sizeof(chaine)-1, 2, chaine);
  
  if (pos < 265) {
    Serial.println(F("Potentiometer calibrated at position : "));
    Serial.print(chaine);

    float val = graphiteSensor();
    dtostrf(val, sizeof(chaine)-1, 2, chaine);
    Serial.println(F("Value : "));
    Serial.print(chaine);
  }
  else {
    Serial.println(F("Potentiometer not calibrated at target 3V"));
    dtostrf(pos, sizeof(chaine)-1, 2, chaine);
    Serial.print(chaine); //à enlever + tard
  }
} 

// Lit la position de l'encodeur, utilisée lors de l'interruption

void doEncoder() {
  if ( (digitalRead(Encodeurclkpin)==HIGH) && (digitalRead(Encodeurdtpin)==HIGH) ) { 
    Pos_encodeur++;
  } 
  else if ( (digitalRead(Encodeurclkpin)==HIGH) && (digitalRead(Encodeurdtpin)==LOW) ) {  //
    Pos_encodeur--;
  }  
}

// Vérifie l'état du bouton de l'endodeur rotatoire

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
  // Serial.println(Menu, DEC);
}

// Affiche le menu sur l'écran OLED et les actions associées

void Afficher_Menu (){
  unsigned long currentMillis = millis ();  // Sauvegarde la valeur du temps écoulé depuis le lancement du programme
  float valeur;
  char place[20];

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
          ecranOLED.setTextColor (SSD1306_WHITE, SSD1306_BLACK);  // Met le texte en blanc et le fond en noir
          ecranOLED.println (F("*MENU*"));
          ecranOLED.setTextSize(1);
          ecranOLED.setTextColor(SSD1306_BLACK, SSD1306_WHITE);
          ecranOLED.println(F("Flex Sensor")); // Ecrit le premier choix du menu comme en "surbrillance"...
          ecranOLED.setTextColor(SSD1306_WHITE, SSD1306_BLACK);
          ecranOLED.println(F("Graphite Sensor")); // ... et le reste normalement
          ecranOLED.println(F("Servomoteur"));
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
          valeur = flexSensor ();
          dtostrf(valeur, sizeof(place)-1, 2, place);
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
            ecranOLED.println(F("Flex Sensor"));
            ecranOLED.setTextColor(SSD1306_BLACK, SSD1306_WHITE);
            ecranOLED.println(F("Graphite Sensor"));
            ecranOLED.setTextColor(SSD1306_WHITE, SSD1306_BLACK);
            ecranOLED.println(F("Servomoteur"));
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
            dtostrf(valeur, sizeof(place)-1, 2, place);
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
          ecranOLED.println(F("Flex Sensor"));
          ecranOLED.println(F("Graphite Sensor"));
          ecranOLED.setTextColor(SSD1306_BLACK, SSD1306_WHITE);
          ecranOLED.println(F("Servomoteur"));
          ecranOLED.display();
        }
        else if (Menu == 1){
          ecranOLED.clearDisplay();   // Effaçage de l'intégralité du buffer
          ecranOLED.setTextSize(2);   // Taille du texte
          ecranOLED.setCursor(0, 0);
          ecranOLED.setTextColor (SSD1306_WHITE, SSD1306_BLACK);
          ecranOLED.println (F("*Menu 3*")); 
          ecranOLED.setTextSize(1);
          ecranOLED.println (F("Servomoteur :")); 
          ecranOLED.setTextSize(1);

          val_flexs=flexSensor();

          if ((abs(val_flexs - lastval))>10){
            //Serial.println (val_flexs);

            if ((val_flexs>34000.0)&&(val_flexs<180000.0)){
              Pos_servo = (val_flexs/112000)*180;
              myservo.write(Pos_servo);
              //Serial.println (Pos_servo);
              dtostrf(Pos_servo, sizeof(place)-1, 2, place);
              ecranOLED.println (place); 
              ecranOLED.display();
              //Serial.println (place);
            }
            else {
              ecranOLED.println ("Verifier la valeur du flex sensor");
              ecranOLED.display();

            }
          }

          lastval=val_flexs;
          

        }
        break;
    }
  }
}