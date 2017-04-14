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
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif

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
# define NUM_LEDS_E 150
# define NUM_LEDS_F 150
# define NUM_LEDS_G 150
# define NUM_LEDS_H 100

const int PINS[] = {PIN_ZONE_A, PIN_ZONE_B, PIN_ZONE_C, PIN_ZONE_D, PIN_ZONE_E, PIN_ZONE_F, PIN_ZONE_G, PIN_ZONE_H};
const int NUM_LEDS[] = {NUM_LEDS_A, NUM_LEDS_B, NUM_LEDS_C, NUM_LEDS_D, NUM_LEDS_E, NUM_LEDS_F, NUM_LEDS_G, NUM_LEDS_H};

/*
  CRGB leds0[NUM_LEDS_A];
  CRGB leds1[NUM_LEDS_B];
  CRGB leds2[NUM_LEDS_C];
  CRGB leds3[NUM_LEDS_D];
*/

Adafruit_NeoPixel pixels_A = Adafruit_NeoPixel(NUM_LEDS[0], PIN_ZONE_A, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel pixels_B = Adafruit_NeoPixel(NUM_LEDS[1], PIN_ZONE_B, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel pixels_C = Adafruit_NeoPixel(NUM_LEDS[2], PIN_ZONE_C, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel pixels_D = Adafruit_NeoPixel(NUM_LEDS[3], PIN_ZONE_D, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel pixels_E = Adafruit_NeoPixel(NUM_LEDS[4], PIN_ZONE_E, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel pixels_F = Adafruit_NeoPixel(NUM_LEDS[5], PIN_ZONE_F, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel pixels_G = Adafruit_NeoPixel(NUM_LEDS[6], PIN_ZONE_G, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel pixels_H = Adafruit_NeoPixel(NUM_LEDS[7], PIN_ZONE_H, NEO_GRB + NEO_KHZ800);


/* Variable de couleur.
  CRGB couleur;
  CRGB couleur_zones[NUM_ZONES];
*/

// Une couleur RGB
int r = 0;
int g = 0;
int b = 0;

// Choix du scénario
int choix = 1;

// Pour les calculs de pulsations
float heartBeat;
float heartFreq = 10000;

int brightness_value = 240; // Between 0 and 100 %

#define FRAMES_PER_SECOND 120

/*********************************************************************************
   Framework FastLED
*********************************************************************************
  //void setPixel(int Pixel, byte red, byte green, byte blue, int NumZone) {
  void setPixel(int Pixel, CRGB c, int NumZone) {
  // FastLED
  leds[NumZone][Pixel] = c;
  }

  //#include "scenarii-leds.h"

  /*********************************************************************************
   Setup
 *********************************************************************************/
void setup()
{

#if defined (__AVR_ATtiny85__)
  if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
#endif

  Serial.begin(9600);
  pinMode(13, OUTPUT); // dry contact
  
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
  /*
    FastLED.addLeds<NEOPIXEL, PIN_ZONE_A>(leds0, NUM_LEDS[0]).setCorrection( Tungsten100W );
    FastLED.addLeds<NEOPIXEL, PIN_ZONE_B>(leds1, NUM_LEDS[1]).setCorrection( Tungsten100W );
    FastLED.addLeds<NEOPIXEL, PIN_ZONE_C>(leds2, NUM_LEDS[2]).setCorrection( Tungsten100W );
    FastLED.addLeds<NEOPIXEL, PIN_ZONE_D>(leds3, NUM_LEDS[3]).setCorrection( Tungsten100W );
  */

  // Diupplication des 4 premières zone pour tester le boitier.
  /*
    FastLED.addLeds<NEOPIXEL, PIN_ZONE_E>(leds0, NUM_LEDS[0]).setCorrection( Tungsten100W );
    FastLED.addLeds<NEOPIXEL, PIN_ZONE_F>(leds1, NUM_LEDS[1]).setCorrection( Tungsten100W );
    FastLED.addLeds<NEOPIXEL, PIN_ZONE_G>(leds2, NUM_LEDS[2]).setCorrection( Tungsten100W );
    FastLED.addLeds<NEOPIXEL, PIN_ZONE_H>(leds3, NUM_LEDS[3]).setCorrection( Tungsten100W );
  */
  pixels_A.begin(); // This initializes the NeoPixel library.
  pixels_B.begin(); // This initializes the NeoPixel library.
  pixels_C.begin(); // This initializes the NeoPixel library.
  pixels_D.begin(); // This initializes the NeoPixel library.
  pixels_E.begin(); // This initializes the NeoPixel library.
  pixels_F.begin(); // This initializes the NeoPixel library.
  pixels_G.begin(); // This initializes the NeoPixel library.
  pixels_H.begin(); // This initializes the NeoPixel library.

  pixels_A.setBrightness(brightness_value);
  pixels_B.setBrightness(brightness_value);
  pixels_C.setBrightness(brightness_value);
  pixels_D.setBrightness(brightness_value);
  pixels_E.setBrightness(brightness_value);
  pixels_F.setBrightness(brightness_value);
  pixels_G.setBrightness(brightness_value);
  pixels_H.setBrightness(brightness_value);

  the_all_black();
  Serial.println("Fin Setup...");
  delay(1000);
  choix = 0;

}

/*********************************************************************************
   BOUCLE
 *********************************************************************************/
void loop() {

  if((millis() % 500) > 250){
    digitalWrite(13, HIGH);
  }else{
    digitalWrite(13, LOW);
  }
  Serial.print("choix = ");
  Serial.print(choix);
  Serial.println();
  
  // Choix d'une zone au hasard
  uint8_t k = random(0, NUM_ZONES);

  // Choix d'un temps d'arrêt aléatoire compris entre 50 ms et 5000ms
  //  uint32_t waiting_time = random(500, 5000);

  // Choix d'une couleur au hasard
  r = random(0, 255);
  g = random(0, 255);
  b = random(0, 255);

  // Choix de la brightness
  brightness_value = random(0, 100);

  // choix du scénario
  switch (choix) {
    case 0 :
      {
        r = 255;
        g = 255;
        b = 255;
        /*
          the_all_black();
          delay(1000);
        */
        setAll(r, g, b, &pixels_A);
        setAll(r, g, b, &pixels_B);
        setAll(r, g, b, &pixels_C);
        setAll(r, g, b, &pixels_D);
        setAll(r, g, b, &pixels_E);
        setAll(r, g, b, &pixels_F);
        setAll(r, g, b, &pixels_G);
        setAll(r, g, b, &pixels_H);
        break;
      }
      
    case 1 :
      // Changer une zone au hasard
        r = 255;
        g = 255;
        b = 255;
        /*
          the_all_black();
          delay(1000);
        */
        setHalf(2, r, g, b, &pixels_A);
        setHalf(2, r, g, b, &pixels_B);
        setHalf(2, r, g, b, &pixels_C);
        setHalf(2, r, g, b, &pixels_D);
        setHalf(2, r, g, b, &pixels_E);
        setHalf(2, r, g, b, &pixels_F);
        setHalf(2, r, g, b, &pixels_G);
        setHalf(2, r, g, b, &pixels_H);

      break;

    case 2 :
        r = 255;
        g = 255;
        b = 255;

        setHalf(3, r, g, b, &pixels_A);
        setHalf(3, r, g, b, &pixels_B);
        setHalf(3, r, g, b, &pixels_C);
        setHalf(3, r, g, b, &pixels_D);
        setHalf(3, r, g, b, &pixels_E);
        setHalf(3, r, g, b, &pixels_F);
        setHalf(3, r, g, b, &pixels_G);
        setHalf(3, r, g, b, &pixels_H);
        
        break;

    case 3 :
      r = 100;
      g = 100;
      b = 150;

      setAll(r, g, b, &pixels_A);
      setAll(r, g, b, &pixels_B);
      setAll(r, g, b, &pixels_C);
      setAll(r, g, b, &pixels_D);
      setAll(r, g, b, &pixels_E);
      setAll(r, g, b, &pixels_F);
      setAll(r, g, b, &pixels_G);
      setAll(r, g, b, &pixels_H);

      break;
    case 4 :
      the_all_black();
      break;

  }
}

void the_all_black() {
  // Tout à noir
  setAll(0, 0, 0, &pixels_A);
  setAll(0, 0, 0, &pixels_B);
  setAll(0, 0, 0, &pixels_C);
  setAll(0, 0, 0, &pixels_D);
  setAll(0, 0, 0, &pixels_E);
  setAll(0, 0, 0, &pixels_F);
  setAll(0, 0, 0, &pixels_G);
  setAll(0, 0, 0, &pixels_H);
}

/*****************************************************************
   Interruptions
 *****************************************************************/
void choix1() {
  cli();
  Serial.println("CHOIX = 1");
  sei();
  choix = 1;
}

void choix2() {
  cli();
  Serial.println("CHOIX = 2");
  sei();
  choix = 2;
}

void choix3() {
  cli();
  Serial.println("CHOIX = 3");
  sei();
  choix = 3;
}

void choix4() {
  cli();
  Serial.println("CHOIX = 4");
  sei();
  choix = 4;
}

/*****************************************************************
   Framwork
 *****************************************************************/
void showStrip() {
  // Néopixel
  pixels_A.show();
  pixels_B.show();
  pixels_C.show();
  pixels_D.show();
  pixels_E.show();
  pixels_F.show();
  pixels_G.show();
  pixels_H.show();
}

void setMyPixel(int Pixel, byte red, byte green, byte blue, Adafruit_NeoPixel *strip) {
  strip->setPixelColor(Pixel, pixels_A.Color(red, green, blue));
  /*
    // NeoPixel
    switch (numZone) {
    case 0 :
      pixels_A.setPixelColor(Pixel, pixels_A.Color(red, green, blue));
      break;
    case 1 :
      pixels_B.setPixelColor(Pixel, pixels_B.Color(red, green, blue));
      break;
    case 2 :
      pixels_C.setPixelColor(Pixel, pixels_C.Color(red, green, blue));
      break;
    case 3 :
      pixels_D.setPixelColor(Pixel, pixels_D.Color(red, green, blue));
      break;
    }
  */
}

void setAll(byte red, byte green, byte blue, Adafruit_NeoPixel *strip) {
  for (int i = 0; i < strip->numPixels(); i++ ) {
    setMyPixel(i, red, green, blue, strip);
  }
  strip->show();
}

void setHalf(int modulo, byte red, byte green, byte blue, Adafruit_NeoPixel *strip) {
  for (int i = 0; i < strip->numPixels(); i++ ) {
    if (i % modulo == 0) {
      setMyPixel(i, red, green, blue, strip);
    } else {
      setMyPixel(i, 0, 0, 0, strip);
    }
  }
  strip->show();
}

