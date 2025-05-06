volatile int comptageImpulsion=0;

void setup() {
  Serial.begin(115200); 
  attachInterrupt(0, gestionINT0, RISING);
  gestionINT0();
} 
  
void loop(){ 
}

void gestionINT0() {
  comptageImpulsion=comptageImpulsion+1; 
  Serial.print("Nombre impulsions = ");
  Serial.println(comptageImpulsion); 
}

