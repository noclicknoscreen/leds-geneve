/*********************************************************************************
   Driver de leds pour le plafonier de l'entree
   Projet Geneve
   2017 - Noclick.noscreen_ - Pierre-Gilles Levallois

   TODO :
    - scénario qui fait une vague (sinusoidale de la brillance)
      entre les différentes zones de A à F
    - Scénario qui augmente et diminue doucement une zone.
    - Allumer la LED 13 pour dire quel scénario est en cours
      (1 cligno puis temps mort, 2 cli ou 3 cli)
    - Ajouter une lecture de pin digitale pour le contact sec)

*********************************************************************************/
#include "FastLED.h"

FASTLED_USING_NAMESPACE

// Nombre de zones
#define NUM_ZONES 6

// Signaux de pilotage des zones
# define PIN_ZONE_A 6
# define PIN_ZONE_B 16
# define PIN_ZONE_C 17
# define PIN_ZONE_D 20
# define PIN_ZONE_E 22
# define PIN_ZONE_F 23

// Definir les 6 zones a eclairer
// Valeurs réelles
# define NUM_LEDS_A 86
# define NUM_LEDS_B 74
# define NUM_LEDS_C 104
# define NUM_LEDS_D 89
# define NUM_LEDS_E 85
# define NUM_LEDS_F 84

/* Valeurs pour la maquette
  # define NUM_LEDS_A 22
  # define NUM_LEDS_B 18
  # define NUM_LEDS_C 26
  # define NUM_LEDS_D 22
  # define NUM_LEDS_E 22
  # define NUM_LEDS_F 21
*/

const int PINS[] = {PIN_ZONE_A, PIN_ZONE_B, PIN_ZONE_C, PIN_ZONE_D, PIN_ZONE_E, PIN_ZONE_F};
const int NUM_LEDS[] = {NUM_LEDS_A, NUM_LEDS_B, NUM_LEDS_C, NUM_LEDS_D, NUM_LEDS_E, NUM_LEDS_F};

CRGB leds0[NUM_LEDS_A];
CRGB leds1[NUM_LEDS_B];
CRGB leds2[NUM_LEDS_C];
CRGB leds3[NUM_LEDS_D];
CRGB leds4[NUM_LEDS_E];
CRGB leds5[NUM_LEDS_F];

CRGB * leds[] = {leds0, leds1, leds2, leds3, leds4, leds5};

// Variable de couleur.
CRGB couleur;

// Choix du scénario
uint8_t choix = 1;



int brightness_value = 10; // Between 0 and 100 %
#define BRIGHTNESS         50
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

  FastLED.addLeds<NEOPIXEL, PIN_ZONE_A>(leds0, NUM_LEDS[0]).setCorrection( Candle );
  FastLED.addLeds<NEOPIXEL, PIN_ZONE_B>(leds1, NUM_LEDS[1]).setCorrection( Candle );
  FastLED.addLeds<NEOPIXEL, PIN_ZONE_C>(leds2, NUM_LEDS[2]).setCorrection( Candle );
  FastLED.addLeds<NEOPIXEL, PIN_ZONE_D>(leds3, NUM_LEDS[3]).setCorrection( Candle );
  FastLED.addLeds<NEOPIXEL, PIN_ZONE_E>(leds4, NUM_LEDS[4]).setCorrection( Candle );
  FastLED.addLeds<NEOPIXEL, PIN_ZONE_F>(leds5, NUM_LEDS[5]).setCorrection( Candle );
  //  zoneA = LedsZone(PIN_ZONE_A, 1, ZONE_BG, ZONE_CW, 0, NUM_LEDS[0], 29, 14).init();
  //  zoneB = LedsZone(PIN_ZONE_B, 2, ZONE_BG, ZONE_CW, 0, NUM_LEDS[1], 22, 14).init();
  //  zoneC = LedsZone(PIN_ZONE_C, 3, ZONE_BG, ZONE_CW, 0, NUM_LEDS[2], 37, 14).init();
  //  zoneD = LedsZone(PIN_ZONE_D, 4, ZONE_BG, ZONE_CW, 0, NUM_LEDS[3], 30, 14).init();
  //  zoneE = LedsZone(PIN_ZONE_E, 5, ZONE_BG, ZONE_CW, 0, NUM_LEDS[4], 28, 14).init();
  //  zoneF = LedsZone(PIN_ZONE_F, 6, ZONE_BG, ZONE_CW, 0, NUM_LEDS[5], 28, 14).init();

  FastLED.setBrightness(BRIGHTNESS);

  // Tout à noir
  for (uint16_t i = 0; i < NUM_ZONES; i++) {
    allColor(CRGB::Black, i);
  }
  showStrip();

  // Init de toutes les zones
  for (uint8_t i = 0; i < NUM_ZONES; i++) {
    // couleur au hasard
    allColor(randomColor(), i);
    delay(30);
    showStrip();
  }

  choix = 1;
}

/*********************************************************************************
   BOUCLE
 *********************************************************************************/
void loop() {
  // Choix d'une zone au hasard
  uint8_t k = random(0, NUM_ZONES);

  // Choix d'un temps d'arrêt aléatoire compris entre 50 ms et 5000ms
  uint32_t waiting_time = random(500, 10000);

  // Choix d'une couleur au hasard
  couleur = randomColor();
  
  // Choix de la brightness
  brightness_value = random(0, 100);

  // Debug values
  Serial.println("------------------------------------------------");
  Serial.print("zone : "); Serial.println(k);
  Serial.print("Scénario : "); Serial.println(choix);
  Serial.print("r:"); Serial.print(couleur.r);
  Serial.print(", g:"); Serial.print(couleur.g);
  Serial.print(", b:"); Serial.println(couleur.b);
  Serial.print("Brightness : "); Serial.println(brightness_value);
  Serial.print("Temps d'attente : "); Serial.println(waiting_time);

  // choix du scénario
  switch (choix) {
    case 1 :
      // Changer une zone au hasard
      colorWipe(couleur, 50, k);
      // temps d'arrêt aléatoire compris entre 50 ms et 500ms
      delay(waiting_time);
      break;
    case 2:
      // faire scintiller des leds
      SnowSparkle(CRGB::Yellow, 50, random(100, 1000), k);
      break;
      //    case 3 :
      //      CylonBounce(r, g, b, 3, 60, 60, k);
      //      setAll(r/10, g/10, b/10, k);
      //      break;
  }

}

