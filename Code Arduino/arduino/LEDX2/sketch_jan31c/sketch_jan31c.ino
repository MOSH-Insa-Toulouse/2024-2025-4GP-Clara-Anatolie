unsigned long time12;
unsigned long time13;
boolean etat13 = 0;
boolean etat12 = 0;

int led13 =13;
int led12 =12;

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(led13, OUTPUT);
  pinMode (led12, OUTPUT);
  Serial.begin(9600); 
  Serial.print("yo");
}

// the loop function runs over and over again forever
void loop() {

  if (millis()-time13>500){
    time13=millis();
    etat13=!etat13;
    digitalWrite (led13, etat13);
  }

  if (millis()-time12>250){
    time12=millis();
    etat12=!etat12;
    digitalWrite (led12, etat12);
  }
}