#include <Servo.h> 
int pinMoteur = 9;
Servo myservo;
int Pos_servo = 90;

void setup() {
  myservo.attach(pinMoteur);
}

void loop() {
  Serial.println(F("servo"));
  myservo.write(Pos_servo);
  delay(500);

}
