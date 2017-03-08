/*********************************************************************************
   Driver de leds pour le plafonier de l'entree
   Projet Geneve
   2017 - Noclick.noscreen_ - Pierre-Gilles Levallois

   TODO :
    - scénario qui fait une vague (sinusoidale de la brillance)
      entre les différentes zones de A à F
    - Scénario qui augmente et diminue doucement une zone.
    - Ajouter une lecture de pin digitale pour le contact sec)

*********************************************************************************/
#include "FastLED.h"

FASTLED_USING_NAMESPACE

// Nombre de zones
#define NUM_ZONES 4

// Signaux de pilotage des zones
# define PIN_ZONE_A 3
# define PIN_ZONE_B 4
# define PIN_ZONE_C 5
# define PIN_ZONE_D 6

# define PIN_ZONE_E 9
# define PIN_ZONE_F 10
# define PIN_ZONE_G 21
# define PIN_ZONE_H 22

// Signaux de pilotage des scénarii
#define PIN_SC1 7
#define PIN_SC2 8
#define PIN_SC3 A0
#define PIN_SC4 A1

// Definir les 6 zones a eclairer
// Valeurs réelles
# define NUM_LEDS_A 150
# define NUM_LEDS_B 150
# define NUM_LEDS_C 150
# define NUM_LEDS_D 100

const int PINS[] = {PIN_ZONE_A, PIN_ZONE_B, PIN_ZONE_C, PIN_ZONE_D};
const int NUM_LEDS[] = {NUM_LEDS_A, NUM_LEDS_B, NUM_LEDS_C, NUM_LEDS_D};

CRGB leds0[NUM_LEDS_A];
CRGB leds1[NUM_LEDS_B];
CRGB leds2[NUM_LEDS_C];
CRGB leds3[NUM_LEDS_D];


CRGB * leds[] = {leds0, leds1, leds2, leds3};

// Variable de couleur.
CRGB couleur;
CRGB couleur_zones[NUM_ZONES];

// Choix du scénario
int choix = 1;

// Pour les calculs de pulsations
float heartBeat;
float heartFreq = 10000;

int brightness_value = 240; // Between 0 and 100 %

#define FRAMES_PER_SECOND 120

/*********************************************************************************
   Framework FastLED
*********************************************************************************/
//void setPixel(int Pixel, byte red, byte green, byte blue, int NumZone) {
void setPixel(int Pixel, CRGB c, int NumZone) {
  // FastLED
  leds[NumZone][Pixel] = c;
}

#include "scenarii-leds.h"

/*********************************************************************************
   Setup
 *********************************************************************************/
void setup()
{
  Serial.begin(9600);

  // Pin de pilotage des scénarii
  pinMode(PIN_SC1, INPUT_PULLUP); // dry contact
  pinMode(PIN_SC2, INPUT_PULLUP); // dry contact
  pinMode(PIN_SC3, INPUT_PULLUP); // dry contact
  pinMode(PIN_SC4, INPUT_PULLUP); // dry contact

  // Interruptions sur les pins de choix de scénario
  //RISING/HIGH/CHANGE/LOW/FALLING
  attachInterrupt (PIN_SC1, choix1, FALLING);  
  attachInterrupt (PIN_SC2, choix2, FALLING);  
  attachInterrupt (PIN_SC3, choix3, FALLING);  
  attachInterrupt (PIN_SC4, choix4, FALLING);


  // Correction intéressantes : Candle (intime, plus sur les rouges), Tungsten40W, Tungsten100W, ClearBlueSky (tire sur le vert/bleu)
  FastLED.addLeds<NEOPIXEL, PIN_ZONE_A>(leds0, NUM_LEDS[0]).setCorrection( Tungsten100W );
  FastLED.addLeds<NEOPIXEL, PIN_ZONE_B>(leds1, NUM_LEDS[1]).setCorrection( Tungsten100W );
  FastLED.addLeds<NEOPIXEL, PIN_ZONE_C>(leds2, NUM_LEDS[2]).setCorrection( Tungsten100W );
  FastLED.addLeds<NEOPIXEL, PIN_ZONE_D>(leds3, NUM_LEDS[3]).setCorrection( Tungsten100W );

  // Diupplication des 4 premières zone pour tester le boitier.
  FastLED.addLeds<NEOPIXEL, PIN_ZONE_E>(leds0, NUM_LEDS[0]).setCorrection( Tungsten100W );
  FastLED.addLeds<NEOPIXEL, PIN_ZONE_F>(leds1, NUM_LEDS[1]).setCorrection( Tungsten100W );
  FastLED.addLeds<NEOPIXEL, PIN_ZONE_G>(leds2, NUM_LEDS[2]).setCorrection( Tungsten100W );
  FastLED.addLeds<NEOPIXEL, PIN_ZONE_H>(leds3, NUM_LEDS[3]).setCorrection( Tungsten100W );

  FastLED.setBrightness(brightness_value);

  tout_eteindre();

  choix = 0;
}

/*********************************************************************************
   BOUCLE
 *********************************************************************************/
void loop() {

  // Choix d'une zone au hasard
  uint8_t k = random(0, NUM_ZONES);

  // Choix d'un temps d'arrêt aléatoire compris entre 50 ms et 5000ms
  //  uint32_t waiting_time = random(500, 5000);

  // Choix d'une couleur au hasard
  couleur = randomColor();

  // Choix de la brightness
  brightness_value = random(0, 100);

  // Debug values
  Serial.println("------------------------------------------------");
//  Serial.print("zone : "); Serial.println(k);
  Serial.print("Scénario : "); Serial.println(choix);
//  Serial.print("r:"); Serial.print(couleur.r);
//  Serial.print(", g:"); Serial.print(couleur.g);
//  Serial.print(", b:"); Serial.println(couleur.b);
//  Serial.print("Brightness : "); Serial.println(brightness_value);
  Serial.print("FPS : "); Serial.println(FastLED.getFPS(), DEC);
//  Serial.print("Temps d'attente : "); Serial.println(waiting_time);

  // choix du scénario
  switch (choix) {
    case 0 :
      {
        // Scénario de test des zones
//        tout_eteindre();
//        Serial.println("------------------------ Test du paramétrage des zones ------------------------");
//        for (int i = 0; i < NUM_ZONES; i++) {
//          Serial.print("zone : "); Serial.println(i);
//          testZone(randomColor(), 30, 1, i);
//        }
        // Test de toutes les zones comme un seul ruban
        for (int n = 0; n < NUM_ZONES; n++) {
          for (int i = 0; i < NUM_LEDS[n]; i++) {
            leds[n][i] = 0xf0f0f0; // 0xffad08; //couleur_zones[n];
            showStrip();
          }
          //delay(FAST);
        }
        break;
      }
    case 1 :
      // Changer une zone au hasard
      colorWipe(couleur, FASTER, k);
      // temps d'arrêt aléatoire compris entre 50 ms et 500ms
      delay(1000);
      break;
    case 2 :
      // faire scintiller des leds
      SnowSparkle(CRGB::Yellow, 50, 50, k);
      break;
    case 3 :
      {
        // Toutes les zones : éclaraige pulsant sinusoïdale
        int total_led = 0;
        // On fait 100 pulsations
        for (int pulse = 10; pulse < 110; pulse++) {
          // --- Calculation of heartBeat
          heartBeat = fmod(millis(), heartFreq) / heartFreq;
          heartBeat = 0.5f * (sin(TWO_PI * heartBeat) + 1);
          brightness_value = 100.0f * heartBeat;
          FastLED.setBrightness(brightness_value);
          for (int n = 0; n < NUM_ZONES; n++) {
            total_led += NUM_LEDS[n];
            for (int i = 0; i < NUM_LEDS[n]; i++) {
              leds[n][i] = 0xffad08; //couleur_zones[n];
            }
          }
          showStrip();
          //delay(FASTER);
        }
        break;
      }
    case 4 :
      tout_eteindre();
      break;

  }

}

void tout_eteindre() {
  // Tout à noir
  for (uint16_t i = 0; i < NUM_ZONES; i++) {
    allColor(CRGB::Black, i);
  }
  showStrip();

}

/*****************************************************************
 * Interruptions 
 *****************************************************************/ 
void choix1(){
  cli();
  Serial.println("CHOIX = 1");
  sei();
  choix=1;
}
 
void choix2(){
  cli();
  Serial.println("CHOIX = 2");
  sei();
  choix=2;
}
 
void choix3(){
  cli();
  Serial.println("CHOIX = 3");
  sei();
  choix=3;
}
 
void choix4(){
  cli();
  Serial.println("CHOIX = 4");
  sei();
  choix=4;
}
 

