#include <LiquidCrystal.h> 

const int numRows = 2; 
const int numCols = 16; 

/* Le montage:  Afficheur LCD  
* LCD RS     - pin 2  
* LCD Enable - pin 3  
* LCD D4     - pin 4  
* LCD D5     - pin 5 
* LCD D6     - pin 6 
* LCD D7     - pin 7 
* LCD R/W    - GND 
*/
LiquidCrystal lcd(2,3,4,5,6,7); 

void setup() 
{   
Serial.begin(9600);
lcd.begin(numCols,numRows);     
lcd.print( "demo LCD" );   
lcd.setCursor(0,1);  
lcd.print( "Hello!" );    
delay(2000);
lcd.clear();
delay(10); 
lcd.blink();
}

void loop()
{
lcd.setCursor(0, 0) ; 
lcd.print("Eh mademoiselle") ;
delay(1000); 
lcd.clear(); 
}
