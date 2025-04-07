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

  readRotaryEncoder();

  ClickEncoder::Button b = encoder->getButton(); //à comprendre: ça fait quoi?
   if (b != ClickEncoder::Open) {
   switch (b) {
      case ClickEncoder::Clicked:
         middle=true;
        break;
    }
  }

  if (up && page == 1 ) {
     
    up = false;
    if(menuitem==2 && frame ==2)
    {
      frame--;
    }

     if(menuitem==4 && frame ==4)
    {
      frame--;
    }
      if(menuitem==3 && frame ==3)
    {
      frame--;
    }
    lastMenuItem = menuitem;
    menuitem--;
    if (menuitem==0)
    {
      menuitem=1;
    } 
  }else if (up && page == 2 && menuitem==1 ) {
    up = false;
    contrast--;
    setContrast();
  }
  else if (up && page == 2 && menuitem==2 ) {
    up = false;
    volume--;
  }
  else if (up && page == 2 && menuitem==3 ) {
    up = false;
    selectedLanguage--;
    if(selectedLanguage == -1)
    {
      selectedLanguage = 2;
    }
  }
    else if (up && page == 2 && menuitem==4 ) {
    up = false;
    selectedDifficulty--;
    if(selectedDifficulty == -1)
    {
      selectedDifficulty = 1;
    }
  }

  if (down && page == 1) //We have turned the Rotary Encoder Clockwise
  {

    down = false;
    if(menuitem==3 && lastMenuItem == 2)
    {
      frame ++;
    }else  if(menuitem==4 && lastMenuItem == 3)
    {
      frame ++;
    }
     else  if(menuitem==5 && lastMenuItem == 4 && frame!=4)
    {
      frame ++;
    }
    lastMenuItem = menuitem;
    menuitem++;  
    if (menuitem==7) 
    {
      menuitem--;
    }
  
  }else if (down && page == 2 && menuitem==1) {
    down = false;
    contrast++;
    setContrast();
  }
  else if (down && page == 2 && menuitem==2) {
    down = false;
    volume++;
  }
   else if (down && page == 2 && menuitem==3 ) {
    down = false;
    selectedLanguage++;
    if(selectedLanguage == 3)
    {
      selectedLanguage = 0;
    }
  }
  else if (down && page == 2 && menuitem==4 ) {
    down = false;
    selectedDifficulty++;
    if(selectedDifficulty == 2)
    {
      selectedDifficulty = 0;
    }
  }
  
  if (middle) //Middle Button is Pressed
  {
    middle = false;
   
    if (page == 1 && menuitem==5) // Backlight Control 
    {
      if (backlight) 
      {
        backlight = false;
        menuItem5 = "Light: OFF";
        turnBacklightOff();
        }
      else 
      {
        backlight = true; 
        menuItem5 = "Light: ON";
        turnBacklightOn();
       }
    }

    if(page == 1 && menuitem ==6)// Reset
    {
      resetDefaults();
    }


    else if (page == 1 && menuitem<=4) {
      page=2;
     }
      else if (page == 2) 
     {
      page=1; 
     }
   }   
  }
  val_flexs = analogRead(flexPin);


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
