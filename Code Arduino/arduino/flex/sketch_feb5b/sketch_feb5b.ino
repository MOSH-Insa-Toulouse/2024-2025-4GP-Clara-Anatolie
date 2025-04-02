const int ledPin = 3;
const int flexPin = A0; //pin A0 to read analog input

const float vcc=5;
const float rdiv=39000.0;
const float flatres=25000;
const float bendres=100000;

int value; //save analog value

void setup(){
  Serial.begin(9600);
  pinMode(flexPin, INPUT);
}

void loop(){
  value = analogRead(flexPin);
  analogWrite(ledPin, value);
  float vflex=value*vcc/1023;
  float rflex=rdiv*(vcc/vflex-1);      //Read and save analog value from potentiometer
  Serial.println("resistance:" + String(rflex) + "ohms");
  Serial.println("voltage:" + String(vflex) + "volts");
  value = map(rflex, 700, 900, 0, 255);
  value = map(vflex, 700, 900, 0, 255);//Map value 0-1023 to 0-255 (PWM)
  delay(100);                          //Small delay
}