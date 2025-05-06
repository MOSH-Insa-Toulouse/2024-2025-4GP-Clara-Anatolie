int pinBouton = 2; 
int etatBouton; 
int lecture1, lecture2; 

void changeEtatBouton( int nouvelEtat ){ 
if( nouvelEtat == LOW ) {
  Serial.print( "Bouton relache !\n" ); }
else {
  Serial.print( "Bouton enfonce !\n" );}
etatBouton=nouvelEtat; 
} 


void setup() {
  // put your setup code here, to run once:
pinMode(pinBouton, INPUT);
etatBouton=digitalRead( pinBouton );

Serial.begin(9600); 
Serial.print("Appuie sur le champignon");

}

void loop() {
  // put your main code here, to run repeatedly:



lecture1=digitalRead(pinBouton); 
delay(10); 
lecture2=digitalRead(pinBouton); 

// changement d'état ?   
if( (lecture1==lecture2) && (lecture1 != etatBouton) ) {
  changeEtatBouton(lecture1); 
} 





}