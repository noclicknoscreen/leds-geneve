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
#include <Zones.h>
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

// Definir les 6 zones à éclairer
// Valeurs réelles
# define NUM_LEDS_A 150
# define NUM_LEDS_B 150
# define NUM_LEDS_C 150
# define NUM_LEDS_D 100
# define NUM_LEDS_E 150
# define NUM_LEDS_F 150
# define NUM_LEDS_G 150
# define NUM_LEDS_H 150

const int PINS[] = {PIN_ZONE_A, PIN_ZONE_B, PIN_ZONE_C, PIN_ZONE_D, PIN_ZONE_E, PIN_ZONE_F, PIN_ZONE_G, PIN_ZONE_H};
//const int NUM_LEDS[] = {NUM_LEDS_A, NUM_LEDS_B, NUM_LEDS_C, NUM_LEDS_D, NUM_LEDS_E, NUM_LEDS_F, NUM_LEDS_G, NUM_LEDS_H};

Adafruit_NeoPixel pixels_A = Adafruit_NeoPixel(NUM_LEDS_A, PIN_ZONE_A, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel pixels_B = Adafruit_NeoPixel(NUM_LEDS_B, PIN_ZONE_B, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel pixels_C = Adafruit_NeoPixel(NUM_LEDS_C, PIN_ZONE_C, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel pixels_D = Adafruit_NeoPixel(NUM_LEDS_D, PIN_ZONE_D, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel pixels_E = Adafruit_NeoPixel(NUM_LEDS_E, PIN_ZONE_E, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel pixels_F = Adafruit_NeoPixel(NUM_LEDS_F, PIN_ZONE_F, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel pixels_G = Adafruit_NeoPixel(NUM_LEDS_G, PIN_ZONE_G, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel pixels_H = Adafruit_NeoPixel(NUM_LEDS_H, PIN_ZONE_H, NEO_GRB + NEO_KHZ800);

Zone zoneA = Zone(&pixels_C, 1, 20);
Zone zoneA1 = Zone(&pixels_C, 21, 40);
Zone zoneB0 = Zone(&pixels_B, 1, 60);
Zone zoneB1 = Zone(&pixels_B, 61, 100);

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

/*********************************************************************************
   Setup
 *********************************************************************************/
void setup()
{

#if defined (__AVR_ATtiny85__)
  if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
#endif

  //Serial.begin(9600);

  // Pin de pilotage des scénarii
  pinMode(PIN_SC1, INPUT_PULLUP); // dry contact
  pinMode(PIN_SC2, INPUT_PULLUP); // dry contact
  pinMode(PIN_SC3, INPUT_PULLUP); // dry contact
  pinMode(PIN_SC4, INPUT_PULLUP); // dry contact
  pinMode(13, OUTPUT);

  pinMode(PIN_ZONE_A, LOW);
  pinMode(PIN_ZONE_B, LOW);
  pinMode(PIN_ZONE_C, LOW);
  pinMode(PIN_ZONE_D, LOW);
  pinMode(PIN_ZONE_E, LOW);
  pinMode(PIN_ZONE_F, LOW);
  pinMode(PIN_ZONE_G, LOW);
  pinMode(PIN_ZONE_H, LOW);

  // Interruptions sur les pins de choix de scénario
  //RISING/HIGH/CHANGE/LOW/FALLING
  attachInterrupt (PIN_SC1, choix1, FALLING);
  attachInterrupt (PIN_SC2, choix2, FALLING);
  attachInterrupt (PIN_SC3, choix3, FALLING);
  attachInterrupt (PIN_SC4, choix4, FALLING);

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

  setAllStrips();
  showAllStrips();

  choix = 0;

  digitalWrite(13, HIGH);

}

/*********************************************************************************
   BOUCLE
 *********************************************************************************/
void loop() {

  int myMillis = millis();
  myMillis %= 200;

  if (myMillis >= 100) {
    digitalWrite(13, HIGH);
  } else {
    digitalWrite(13, LOW);
  }

  // choix du scénario
  switch (choix) {
    case 0 :
      {

        zoneA.setZoneColor(255, 255, 255);
        zoneA1.setZoneColor(175, 60, 190);
        zoneB0.setZoneColor(100, 107, 189);
        zoneB1.setZoneColor(175, 82, 90);

        // ------------------------------------------------------
        int freq = 5000;
        float perCent = fmod(millis(), freq) / freq;
        perCent *= 100.0f;

        float zoneAValue, zoneBValue, zoneCValue, zoneDValue;

        zoneAValue = clampMap(perCent,  0,  25,   0, 255);
        zoneBValue = clampMap(perCent,  25,  50,   0, 255);
        zoneCValue = clampMap(perCent,  50, 75,   0, 255);
        zoneDValue = clampMap(perCent,  75,  100,   0, 255);

        Serial.print("Values : ");
        Serial.print(zoneAValue);
        Serial.print(" : ");
        Serial.print(zoneBValue);
        Serial.print(" : ");
        Serial.print(zoneCValue);
        Serial.print(" : ");
        Serial.print(zoneDValue);

        Serial.println();
        /*
          zoneA.setZoneColor(zoneAValue, zoneAValue, zoneAValue);
          zoneA1.setZoneColor(zoneBValue, zoneBValue, zoneBValue);
          zoneB0.setZoneColor(zoneCValue, zoneCValue, zoneCValue);
          zoneB1.setZoneColor(zoneDValue, zoneDValue, zoneDValue);

          zoneA.setZoneBrightness(255);
          zoneA1.setZoneBrightness(255);
          zoneB0.setZoneBrightness(255);
          zoneB1.setZoneBrightness(255);
        */

        brightness_value -= 10;

        Serial.print("brightness_value : ");       Serial.println(brightness_value);
        zoneA.setZoneBrightness(brightness_value);
        zoneA1.setZoneBrightness(brightness_value);
        zoneB0.setZoneBrightness(brightness_value);
        zoneB1.setZoneBrightness(brightness_value);
        delay(2000);

        break;
      }

    case 1 :
      break;

    case 2 :
      break;

    case 3 :
      break;
    case 4 :
      break;

  }
  showAllZones();
  //showAllStrips();
}

void SetAllZones() {
  // Tout à noir
  zoneA.setZoneColor(0, 0, 0);
}

void setAllStrips() {
  for (int i = 0; i < pixels_A.numPixels(); i++ ) {
    pixels_A.setPixelColor(i, 0, 0, 0);
  }
  for (int i = 0; i < pixels_B.numPixels(); i++ ) {
    pixels_B.setPixelColor(i, 0, 0, 0);
  }
  for (int i = 0; i < pixels_C.numPixels(); i++ ) {
    pixels_C.setPixelColor(i, 0, 0, 0);
  }
  for (int i = 0; i < pixels_D.numPixels(); i++ ) {
    pixels_D.setPixelColor(i, 0, 0, 0);
  }
  for (int i = 0; i < pixels_E.numPixels(); i++ ) {
    pixels_E.setPixelColor(i, 0, 0, 0);
  }
  for (int i = 0; i < pixels_F.numPixels(); i++ ) {
    pixels_F.setPixelColor(i, 0, 0, 0);
  }
  for (int i = 0; i < pixels_G.numPixels(); i++ ) {
    pixels_G.setPixelColor(i, 0, 0, 0);
  }
  for (int i = 0; i < pixels_H.numPixels(); i++ ) {
    pixels_H.setPixelColor(i, 0, 0, 0);
  }
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
void showAllZones() {
  // Néopixel
  zoneA.show();
  zoneA1.show();
  zoneB0.show();
  zoneB1.show();
}

void showAllStrips() {
  pixels_A.show();
  pixels_B.show();
  pixels_C.show();
  pixels_D.show();
  pixels_E.show();
  pixels_F.show();
  pixels_G.show();
  pixels_H.show();
}

float clampMap(float value, float inMin, float inMax, float outMin, float outMax) {
  return constrain(map(constrain(value,  inMin,  inMax), inMin, inMax, outMin, outMax), outMin, outMax);
}


