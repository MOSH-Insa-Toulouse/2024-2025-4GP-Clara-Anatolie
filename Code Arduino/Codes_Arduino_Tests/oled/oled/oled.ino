#include <Adafruit_SSD1306.h>

#define nombreDePixelsEnLargeur 128
#define nombreDePixelsEnHauteur 64
#define brocheResetOLED -1
#define adresseI2CecranOLED 0x3C

Adafruit_SSD1306 ecranOLED (nombreDePixelsEnLargeur, nombreDePixelsEnHauteur, &Wire, brocheResetOLED);



void setup()
{
  if (!ecranOLED.begin (SSD1306_SWITCHCAPVCC, adresseI2CecranOLED))
    while(1);

  for(byte tailleDeCaractere=1; tailleDeCaractere <=3; tailleDeCaractere++) { 
    boolean bCouleurInverse = false;
    ecranOLED.clearDisplay();
    ecranOLED.setTextSize (tailleDeCaractere);
    ecranOLED.setCursor(0, 0);

    for (byte numeroDeLigne=1; numeroDeLigne <=8; numeroDeLigne++) {

      if(bCouleurInverse){
      ecranOLED.setTextColor(SSD1306_BLACK, SSD1306_WHITE);
      }

      else{

        ecranOLED.setTextColor (SSD1306_BLACK, SSD1306_WHITE); // Couleur du texte, et couleur au fond
        ecranOLED.setTextColor (SSD1306_WHITE);
        ecranOLED.print ("Ligne ");
        ecranOLED.println(numeroDeLigne);
      }

    }
    ecranOLED.display();
    delay(2000);
  }



}

void loop() {

}