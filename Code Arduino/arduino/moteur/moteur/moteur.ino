int pinMoteur = 9; // pin contrôlant le moteur (PWM pin) 

void setup(){
//En PWM il ne faut pas assigner la pin en output
}

void loop(){
analogWrite( pinMoteur, LOW );
delay(2000);
MoteurVitessePWM();
MoteurAccelerationPWM();
}

// Contrôle de la vitesse du moteur en contrôlant  le % du cycle de service PWM 
// sur  la pin 9 (de 0 à 255 pour  le % du cycle de service ).

void MoteurVitessePWM(){
int vitesse1 = int(255) / 3; // High 33% du cycle  
int vitesse2 = int(255) / 2; // High 50% du cycle  
int vitesse3 = 2 * 255 / 3;
int vitesse4 = (int)255; // High 100% du cycle

analogWrite( pinMoteur, vitesse1 ); delay( 1000 );
analogWrite( pinMoteur, vitesse2 ); delay( 1000 );
analogWrite( pinMoteur, vitesse3 ); delay( 1000 ); 
analogWrite( pinMoteur, vitesse4 );  delay( 1000 );
analogWrite( pinMoteur, vitesse3 );  delay( 1000 );
analogWrite( pinMoteur, vitesse2 ); delay( 1000 );
analogWrite( pinMoteur, vitesse1 ); delay( 1000 );
analogWrite( pinMoteur, LOW );
} 

// Contrôle plus fin de l'accélération du moteur via PWM. 
// NB: Selon la qualité du moteur, celui-ci peut avoir du mal à décoller 
//  lorsque le % du cycle de service est assez bas.
// Un option est d'envoyer une impulsion pour démarrer/décoller le moteur.

void MoteurAccelerationPWM(){ 
// Impulsion de démarrage (75%)  
//analogWrite( pinMoteur, 191 );   
//delay(50);      

// Acceleration  
for( int i = 30; i<= 255; i++ ){
analogWrite( pinMoteur, i );
delay(50);
}

delay( 2000 );

// Deceleration  
for( int i = 255; i>=0; i-- ){
  analogWrite( pinMoteur, i );
  delay(50);}
} 
