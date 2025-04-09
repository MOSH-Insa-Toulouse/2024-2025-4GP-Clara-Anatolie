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
#define Encodeurclkpin  2  //CLK Output A avec interruption
#define Encodeurdtpin 3  //DT Output B
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

  //Encodeur:
int Pos_encodeur = 0; 

  //FlexS:
const int flexPin = A0;
int val_flexs = 0;

  //OLED:
String Item1 = "Mesure Flex Sensor";
String Item2 = "Mesure Capt Graphite";
String Item3 = "Servomotor";

Adafruit_SSD1306 ecranOLED (nombreDePixelsEnLargeur, nombreDePixelsEnHauteur, &Wire, brocheResetOLED);

  //Encodeur:
bool etat_bouton = 0;  // variable pour stocker la lecture de l'etat des boutons

//Fonctions:

void doEncoder() {
  if ( (digitalRead(Encodeurclkpin)==HIGH) && (digitalRead(Encodeurdtpin)==HIGH) ) { 
    Pos_encodeur++;
  } 
  else if ( (digitalRead(Encodeurclkpin)==HIGH) && (digitalRead(Encodeurdtpin)==LOW) ) {  //
    Pos_encodeur--;
  }
  
  Serial.println(Pos_encodeur, DEC);    //Angle = (360 / Encoder_Resolution) * encoder0Pos
  ecranOLED.println (F("Menu:"));
  ecranOLED.display();
}

void appui_bouton (){
  etat_bouton = digitalRead(Encodeurswpin);
}

void Afficher_Menu (){
  unsigned long previousMillis = 0;
  unsigned long currentMillis = millis ();
  int choix = 0;


  if (currentMillis - previousMillis >= 500){
    previousMillis = currentMillis;
    
    appui_bouton();

    ecranOLED.clearDisplay();   // Effaçage de l'intégralité du buffer
    ecranOLED.setTextSize(2);   // Taille du texte
    ecranOLED.setCursor(0, 0);
    ecranOLED.setTextColor (SSD1306_WHITE, SSD1306_BLACK);
    ecranOLED.println (F("*MENU*"));
    ecranOLED.display();

    choix = Pos_encodeur % nb_item;
    switch (choix){
      case 0 :
        ecranOLED.setTextSize(1.5);
        ecranOLED.setTextColor(SSD1306_BLACK, SSD1306_WHITE);
        ecranOLED.println(Item1);
        ecranOLED.setTextColor(SSD1306_WHITE, SSD1306_BLACK);
        ecranOLED.println(Item2);
        ecranOLED.println(Item3);
        ecranOLED.display();

        if (etat_bouton == 1){
          val_flexs = analogRead(flexPin);
          Serial.println(val_flexs, DEC);
        }
        break;

      case 1:
        ecranOLED.setTextSize(1.5);
        ecranOLED.setTextColor(SSD1306_WHITE, SSD1306_BLACK);
        ecranOLED.println(Item1);
        ecranOLED.setTextColor(SSD1306_BLACK, SSD1306_WHITE);
        ecranOLED.println(Item2);
        ecranOLED.setTextColor(SSD1306_WHITE, SSD1306_BLACK);
        ecranOLED.println(Item3);
        ecranOLED.display();

        if (etat_bouton == 1){
          Serial.println(F("MESURE GRAPHITE"));
        }
        break;

      case 2:
        ecranOLED.setTextSize(1.5);
        ecranOLED.setTextColor(SSD1306_WHITE, SSD1306_BLACK);
        ecranOLED.println(Item1);
        ecranOLED.println(Item2);
        ecranOLED.setTextColor(SSD1306_BLACK, SSD1306_WHITE);
        ecranOLED.println(Item3);
        ecranOLED.display();

        if (etat_bouton == 1){
          Serial.println(F("servo"));
        }
        break;
    }
  }
}

void setup() {
  //Encodeur rotatoire:
  pinMode(Encodeurclkpin, INPUT); 
  digitalWrite(Encodeurclkpin, HIGH);  // Turn on pullup resistor

  pinMode(Encodeurdtpin, INPUT); 
  digitalWrite(Encodeurdtpin, HIGH);  // Turn on pullup resistor

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

  //OLED:
  if (!ecranOLED.begin (SSD1306_SWITCHCAPVCC, adresseI2CecranOLED)) // Arrêt du programme (boucle infinie) si échec d'initialisation
    while(1);

  //Pour indiquer qu'on démarre:
  Serial.println(F("Let's go"));

}

void loop() {
  //int instr [100];
  //int i = 0;

  Serial.println(F("ça marche ou quoi"));
  Afficher_Menu();

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
