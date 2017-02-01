/*********************************************************************************
   Driver de leds pour le plafonier de l'entree
   Projet Geneve
   2017 - Noclick.noscreen_ - Pierre-Gilles Levallois
*********************************************************************************/
#include "FastLED.h"
#define NUM_ZONES 6
// Definir les 6 zones a eclairer
/*
  typedef struct
  {
  String name;
  const int pin;
  const int numleds;
  } zone;

  const zone zones[1] = {
  { "A", 3, 22 }
  };
*/
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

/*********************************************************************************
 * Setup 
 *********************************************************************************/
void setup()
{
  //FastLED.addLeds<WS2811, PIN_ZONE_A, GRB>(leds0, NUM_LEDS[0]).setCorrection( TypicalLEDStrip );
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
    // Init de toutes les zones
    for (uint16_t i = 0; i < NUM_ZONES; i++) {
      colorWipe(0x70, 0xa2, 0x08, 50, i);
      delay(50);
    }

}

/*********************************************************************************
 * BOUCLE
 *********************************************************************************/
void loop() {
  // Changer une zone au hasard
  uint16_t k = random(0, 6);
  colorWipe(0x00, 0x00, 0x00, 50, k);
  // temps d'arrêt aléatoire compris entre 50 ms et 500ms
  delay(random(50, 5000));
}


/*********************************************************************************
 * ColorWipe : Allumer toutes les leds une à une sur toute la longueur du ruban
 * dans l'ordre et avec une coleur unique, mais choisi au hasard.
 *********************************************************************************/
void colorWipe(byte red, byte green, byte blue, int SpeedDelay, int NumZone) {
  // Choix d'une couleur aléatoire
  red = random(0, 255);
  green = random(0, 255);
  blue = random(0, 255);

  for (uint16_t i = 0; i < NUM_LEDS[NumZone]; i++) {
    setPixel(i, red, green, blue, NumZone);
    showStrip();
    delay(SpeedDelay);
  }
}

/*********************************************************************************
 * Framework d'abtraction NEOPIXEL / FastLED
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
