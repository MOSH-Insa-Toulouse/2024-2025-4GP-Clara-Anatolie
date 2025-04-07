#include <SoftwareSerial.h>
#define rxPin 11  
#define txPin 10
#define baudrate 9600
SoftwareSerial mySerial (rxPin, txPin);

void setup() {
  pinMode (rxPin,INPUT);
  pinMode (txPin,OUTPUT);

  mySerial.begin(baudrate);
  Serial.begin(baudrate);

}

void loop() {
  int i = 0;
  char someChar[32]={0};

  while (Serial.available()){
    do{
      someChar[i++] = Serial.read();
    delay(3);
    }while (Serial.available()>0);

    mySerial.println(someChar);
    Serial.println(someChar);
  }
  while (mySerial.available()){
    Serial.print((char)mySerial.read());
  }
  
  }
      