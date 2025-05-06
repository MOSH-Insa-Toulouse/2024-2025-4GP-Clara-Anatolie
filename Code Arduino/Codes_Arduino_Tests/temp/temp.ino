int temperaturePin = 0; //the analog pin the TMP36′s Vout (sense) pin is 
connected to

void setup(){
  Serial.begin(9600);
}

void loop(){
  float temperature = getVoltage(temperaturePin);
  temperature=(temperature–0,5)*100;
  Serial.println(temperature);
  delay(1000);
}
float getVoltage(int pin){
  return (analogRead(pin)*,004882814);
}