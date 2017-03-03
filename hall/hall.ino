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

#define NUM_ZONES 6
// Definir les 6 zones a eclairer

# define NUM_LEDS_A 22
# define NUM_LEDS_B 18
# define NUM_LEDS_C 26
# define NUM_LEDS_D 22
# define NUM_LEDS_E 22
# define NUM_LEDS_F 21

# define PIN_ZONE_A 6
# define PIN_ZONE_B 16
# define PIN_ZONE_C 17
# define PIN_ZONE_D 20
# define PIN_ZONE_E 22
# define PIN_ZONE_F 23

const int PINS[] = {PIN_ZONE_A, PIN_ZONE_B, PIN_ZONE_C, PIN_ZONE_D, PIN_ZONE_E, PIN_ZONE_F};
const int NUM_LEDS[] = {NUM_LEDS_A, NUM_LEDS_B, NUM_LEDS_C, NUM_LEDS_D, NUM_LEDS_E, NUM_LEDS_F};

CRGB leds0[NUM_LEDS_A];
CRGB leds1[NUM_LEDS_B];
CRGB leds2[NUM_LEDS_C];
CRGB leds3[NUM_LEDS_D];
CRGB leds4[NUM_LEDS_E];
CRGB leds5[NUM_LEDS_F];

// Choix du scénario
uint8_t choix = 1;

byte r;
byte g;
byte b;

int brightness_value = 10; // Between 0 and 100 %
/*********************************************************************************
   Setup
 *********************************************************************************/
void setup()
{
  /*
    FastLED.addLeds<NEOPIXEL, PIN_ZONE_A>(leds0, NUM_LEDS[0]);
    FastLED.addLeds<NEOPIXEL, PIN_ZONE_B>(leds1, NUM_LEDS[1]);
    FastLED.addLeds<NEOPIXEL, PIN_ZONE_C>(leds2, NUM_LEDS[2]);
    FastLED.addLeds<NEOPIXEL, PIN_ZONE_D>(leds3, NUM_LEDS[3]);
    FastLED.addLeds<NEOPIXEL, PIN_ZONE_E>(leds4, NUM_LEDS[4]);
    FastLED.addLeds<NEOPIXEL, PIN_ZONE_F>(leds5, NUM_LEDS[5]);
  */
  FastLED.addLeds<WS2811, PIN_ZONE_A, GRB>(leds0, NUM_LEDS[0]).setCorrection( TypicalLEDStrip );
  FastLED.addLeds<WS2811, PIN_ZONE_B, GRB>(leds1, NUM_LEDS[1]).setCorrection( TypicalLEDStrip );
  FastLED.addLeds<WS2811, PIN_ZONE_C, GRB>(leds2, NUM_LEDS[2]).setCorrection( TypicalLEDStrip );
  FastLED.addLeds<WS2811, PIN_ZONE_D, GRB>(leds3, NUM_LEDS[3]).setCorrection( TypicalLEDStrip );
  FastLED.addLeds<WS2811, PIN_ZONE_E, GRB>(leds4, NUM_LEDS[4]).setCorrection( TypicalLEDStrip );
  FastLED.addLeds<WS2811, PIN_ZONE_F, GRB>(leds5, NUM_LEDS[5]).setCorrection( TypicalLEDStrip );
  // ici il faudra intégrer l'appel au helper qui permet de changer les orientations des zones.

  // Tout à noir
  for (uint16_t i = 0; i < NUM_ZONES; i++) {
    setAll(0, 0, 0, i);
  }
  showStrip();

  // Init de toutes les zones
  for (uint8_t i = 0; i < NUM_ZONES; i++) {
    // couleur au hasard
    r = byte(random(0, 255));
    g = byte(random(0, 255));
    b = byte(random(0, 255));
    setAll(r, g, b, i);
    delay(30);
    showStrip();
  }

  choix = 1;
  // debug
  Serial.begin(9600);
}

/*********************************************************************************
   BOUCLE
 *********************************************************************************/
void loop() {
  // Choix d'une zone au hasard
  uint8_t k = random(0, 6);
  
  // Choix d'un temps d'arrêt aléatoire compris entre 50 ms et 5000ms
  uint32_t waiting_time = random(500, 10000);
  
  // Choix d'une couleur au hasard
  r = byte(random(0, 255));
  g = byte(random(0, 255));
  b = byte(random(0, 255));

  // Choix de la brightness
  brightness_value = random(0, 100);

  // Debug values
  Serial.println("------------------------------------------------");
  Serial.print("zone : "); Serial.println(k);
  Serial.print("Scénario : "); Serial.println(choix);
  Serial.print("r:"); Serial.print(r);
  Serial.print(", g:"); Serial.print(g);
  Serial.print(", b:"); Serial.println(b);
  Serial.print("Brightness : "); Serial.println(brightness_value);
  Serial.print("Temps d'attente : "); Serial.println(waiting_time);
  
  // choix du scénario
  switch (choix) {
    case 1 :
      // Changer une zone au hasard
      colorWipe(r, g, b, 50, k);
      // temps d'arrêt aléatoire compris entre 50 ms et 500ms
      delay(waiting_time);
      break;
    case 2:
      // faire scintiller des leds
      SnowSparkle(0x30, 0x30, 0x30, 50, random(100, 1000), k);
      break;
      //    case 3 :
      //      CylonBounce(r, g, b, 3, 60, 60, k);
      //      setAll(r/10, g/10, b/10, k);
      //      break;
  }

}

/*
   Cyclon

  void CylonBounce(byte red, byte green, byte blue, int EyeSize, int SpeedDelay, int ReturnDelay, int NumZone) {

  for (int i = 0; i < NUM_LEDS[NumZone] - EyeSize - 2; i++) {
    setAll(0, 0, 0, NumZone);
    setPixel(i, red / 10, green / 10, blue / 10, NumZone);
    for (int j = 1; j <= EyeSize; j++) {
      setPixel(i + j, red, green, blue, NumZone);
    }
    setPixel(i + EyeSize + 1, red / 10, green / 10, blue / 10, NumZone);
    showStrip();
    delay(SpeedDelay);
  }

  delay(ReturnDelay);

  for (int i = NUM_LEDS[NumZone] - EyeSize - 2; i > 0; i--) {
    setAll(0, 0, 0, NumZone);
    setPixel(i, red / 10, green / 10, blue / 10, NumZone);
    for (int j = 1; j <= EyeSize; j++) {
      setPixel(i + j, red, green, blue, NumZone);
    }
    setPixel(i + EyeSize + 1, red / 10, green / 10, blue / 10, NumZone);
    showStrip();
    delay(SpeedDelay);
  }

  delay(ReturnDelay);
  }
*/
/*********************************************************************************
   ColorWipe : Allumer toutes les leds une à une sur toute la longueur du ruban
   dans l'ordre et avec une coleur unique, mais choisi au hasard.
 *********************************************************************************/
void colorWipe(byte red, byte green, byte blue, int SpeedDelay, int NumZone) {
  // Choix d'une couleur aléatoire
  for (uint16_t i = 0; i < NUM_LEDS[NumZone]; i++) {
    setPixel(i, red, green, blue, NumZone);
    showStrip();
    delay(SpeedDelay);
  }
}

/*********************************************************************************
   SnowSparkle : Allumer toutes les leds ave une couleur choisie, puis faire
   scintiller des leds au hasard dans la zone.
 *********************************************************************************/
void SnowSparkle(byte red, byte green, byte blue, int SparkleDelay, int SpeedDelay, int NumZone) {
  red = byte(int(red * 0.5));
  green = byte(int(green * 0.5));
  blue = byte(int(blue * 0.5));
  setAll(red, green, blue, NumZone);
  // Choisir un pixel au hasard
  int Pixel = random(NUM_LEDS[NumZone]);
  // TODO :Choisir la couleur la plus brillante dans le respect de la couleur de départ
  setPixel(Pixel, 0xff, 0xff, 0xff, NumZone);
  showStrip();
  delay(SparkleDelay);
  setPixel(Pixel, red, green, blue, NumZone);
  showStrip();
  delay(SpeedDelay);
}
/*********************************************************************************
   Framework d'abtraction NEOPIXEL / FastLED
*********************************************************************************/
void showStrip() {
#ifdef ADAFRUIT_NEOPIXEL_H
  // NeoPixel
  strip.show();
#endif
#ifndef ADAFRUIT_NEOPIXEL_H
  // FastLED
  FastLED.show();
#endif
}

void setPixel(int Pixel, byte red, byte green, byte blue, int NumZone) {
  // Setting Brightness
  red = red * brightness_value / 100;
  green = green * brightness_value / 100;
  blue = blue * brightness_value / 100;

#ifdef ADAFRUIT_NEOPIXEL_H
  // NeoPixel
  strip.setPixelColor(Pixel, strip.Color(red, green, blue));
#endif
#ifndef ADAFRUIT_NEOPIXEL_H
  // FastLED
  switch (NumZone) {
    case 0 :
      leds0[Pixel].r = red;
      leds0[Pixel].g = green;
      leds0[Pixel].b = blue;
      break;
    case 1 :
      leds1[Pixel].r = red;
      leds1[Pixel].g = green;
      leds1[Pixel].b = blue;
      break;
    case 2 :
      leds2[Pixel].r = red;
      leds2[Pixel].g = green;
      leds2[Pixel].b = blue;
      break;
    case 3 :
      leds3[Pixel].r = red;
      leds3[Pixel].g = green;
      leds3[Pixel].b = blue;
      break;
    case 4 :
      leds4[Pixel].r = red;
      leds4[Pixel].g = green;
      leds4[Pixel].b = blue;
      break;
    case 5 :
      leds5[Pixel].r = red;
      leds5[Pixel].g = green;
      leds5[Pixel].b = blue;
      break;
  }

#endif
}

void setAll(byte red, byte green, byte blue, int NumZone) {
  for (int i = 0; i < NUM_LEDS[NumZone]; i++ ) {
    setPixel(i, red, green, blue, NumZone);
  }
  showStrip();
}
