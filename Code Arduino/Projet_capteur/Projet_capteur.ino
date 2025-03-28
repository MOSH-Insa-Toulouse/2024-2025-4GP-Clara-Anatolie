//Inclusion des bibliothèques:
#include <SoftwareSerial.h>  //Bluetooth
#include <Servo.h>  //Servo
#include <Wire.h>   //Encodeur + OLED
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>
#include <ClickEncoder.h>
#include <TimerOne.h>
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

  //Menu OLED:
String menuItem1 = "Mesure avec Flex Sensor";
String menuItem2 = "Mesure avec graphite";
String menuItem3 = "Autre";
String menuItem4 = "Reset";

int page = 1;
int frame = 1;
int menuitem = 1;
int lastMenuItem = 1;

//Fonctions:

/* void doEncoder() {
  if ( (digitalRead(Encodeurclk)==HIGH) && (digitalRead(Encodeurdt)==HIGH)) { 
    Pos_encodeur++;
  } else if ( (digitalRead(Encodeurclk)==HIGH) && (digitalRead(Encodeurdt)==LOW)) {  //
    Pos_encodeur--;
  }
  // Serial.println (encoder0Pos, DEC);  //Angle = (360 / Encoder_Resolution) * encoder0Pos
} */
void displayMenuItem(String item, int position, boolean selected)
{
    if(selected)
    {
      display.setTextColor(BLACK,WHITE);//, BLACK
    }else
    {
      display.setTextColor(WHITE);//BLACK,
    }
    display.setCursor(0, position);
    display.print(">"+item);
}

void readRotaryEncoder()
{
  value += encoder->getValue();
  
  if (value/2 > last) {
    last = value/2;
    down = true;
    delay(150);
  }
  else   if (value/2 < last) {
    last = value/2;
    up = true;
    delay(150);
  }
}

void drawMenu() {

  display.setTextSize(1);
  display.clearDisplay();
  display.setTextColor(WHITE,BLACK );
  display.setCursor(15, 0);
  display.println("    MAIN MENU");
  display.drawFastHLine(0,10,128,WHITE);//BLACK

  if(menuitem==1 && frame ==1)
    {   
      displayMenuItem(menuItem1, 15,true);
      displayMenuItem(menuItem2, 25,false);
      displayMenuItem(menuItem3, 35,false);
    }
    else if(menuitem == 2 && frame == 1)
    {
      displayMenuItem(menuItem1, 15,false);
      displayMenuItem(menuItem2, 25,true);
      displayMenuItem(menuItem3, 35,false);
    }
    else if(menuitem == 3 && frame == 1)
    {
      displayMenuItem(menuItem1, 15,false);
      displayMenuItem(menuItem2, 25,false);
      displayMenuItem(menuItem3, 35,true);
    }
     else if(menuitem == 4 && frame == 2)
    {
      displayMenuItem(menuItem2, 15,false);
      displayMenuItem(menuItem3, 25,false);
      displayMenuItem(menuItem4, 35,true);
    }

      else if(menuitem == 3 && frame == 2)
    {
      displayMenuItem(menuItem2, 15,false);
      displayMenuItem(menuItem3, 25,true);
      displayMenuItem(menuItem4, 35,false);
    }
    else if(menuitem == 2 && frame == 2)
    {
      displayMenuItem(menuItem2, 15,true);
      displayMenuItem(menuItem3, 25,false);
      displayMenuItem(menuItem4, 35,false);
    }
    display.display();
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
  char instr [100];
  int i = 0;

  drawMenu();

  //value = analogRead(flexPin);
  /*Serial.println (Pos_encodeur, DEC);  //Angle = (360 / Encoder_Resolution) * encoder0Pos
  if (Pos_encodeur >= 30){
    Pos_encodeur = 0;
  }
  Pos_servo=160/30*Pos_encodeur; 
  myservo.write(Pos_servo);
  */
  
  //char someChar[32]={0};

  if (Serial.available()>0){
    instr=Serial.read();
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
