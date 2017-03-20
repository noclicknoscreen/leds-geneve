/*********************************************************************************
   Driver de leds pour le plafonier du hall DIALIN
   Projet Geneve
   2017 - Noclick.noscreen_ - Pierre-Gilles Levallois - Sebastien Albert

*********************************************************************************/
#include <Adafruit_NeoPixel.h>
#include <Zones.h>
#ifdef __AVR__
#include <avr/power.h>
#endif

// Nombre de zones
#define NUM_ZONES 8

// on board Led
#define TEENSY_LED 13
// Signaux de pilotage des zones
# define PIN_ZONE_C1 3
# define PIN_ZONE_C2 4
# define PIN_ZONE_C3 5
# define PIN_ZONE_C4 6
# define PIN_ZONE_C5 9
# define PIN_ZONE_C6 10
# define PIN_ZONE_C7 22  // 
# define PIN_ZONE_C8 22  // 

// Definir les 6 zones a eclairer
// Valeurs réelles
# define NUM_LEDS_C1 43
# define NUM_LEDS_C2 43
# define NUM_LEDS_C3 43
# define NUM_LEDS_C4 43 
# define NUM_LEDS_C5 43
# define NUM_LEDS_C6 43 // OK
# define NUM_LEDS_C7 43
# define NUM_LEDS_C8 43 // OK

const int PINS[NUM_ZONES] = {PIN_ZONE_C1, PIN_ZONE_C2, PIN_ZONE_C3, PIN_ZONE_C4, PIN_ZONE_C5, PIN_ZONE_C6, PIN_ZONE_C8};

Adafruit_NeoPixel pixels_C1 = Adafruit_NeoPixel(NUM_LEDS_C1, PIN_ZONE_C1, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel pixels_C2 = Adafruit_NeoPixel(NUM_LEDS_C2, PIN_ZONE_C2, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel pixels_C3 = Adafruit_NeoPixel(NUM_LEDS_C3, PIN_ZONE_C3, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel pixels_C4 = Adafruit_NeoPixel(NUM_LEDS_C4, PIN_ZONE_C4, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel pixels_C5 = Adafruit_NeoPixel(NUM_LEDS_C5, PIN_ZONE_C5, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel pixels_C6 = Adafruit_NeoPixel(NUM_LEDS_C6, PIN_ZONE_C6, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel pixels_C7 = Adafruit_NeoPixel(NUM_LEDS_C7, PIN_ZONE_C7, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel pixels_C8 = Adafruit_NeoPixel(NUM_LEDS_C8, PIN_ZONE_C8, NEO_GRB + NEO_KHZ800);

Zone zone1 = Zone(&pixels_C1, 1, NUM_LEDS_C1);
Zone zone2 = Zone(&pixels_C2, 1, NUM_LEDS_C2);
Zone zone3 = Zone(&pixels_C3, 1, NUM_LEDS_C3);
Zone zone4 = Zone(&pixels_C4, 1, NUM_LEDS_C4);
Zone zone5 = Zone(&pixels_C5, 1, NUM_LEDS_C5);
Zone zone6 = Zone(&pixels_C6, 1, NUM_LEDS_C6);
Zone zone7 = Zone(&pixels_C7, 1, NUM_LEDS_C7);
Zone zone8 = Zone(&pixels_C8, 1, NUM_LEDS_C8);

Zone * ZONES[NUM_ZONES] = {&zone1, &zone2, &zone3, &zone4, &zone5, &zone6, &zone7, &zone8};

Adafruit_NeoPixel * STRIPS[NUM_ZONES] = {&pixels_C1, &pixels_C2, &pixels_C3, &pixels_C4, &pixels_C5, &pixels_C6, &pixels_C7, &pixels_C8};

// Choix du scénario
int choix = 0;

int brightness_value = 255; // Between 0 and 100 %

/*********************************************************************************
   Setup
 *********************************************************************************/
void setup()
{
#if defined (__AVR_ATtiny85__)
  if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
#endif
  Serial.begin(9600);
  Serial.println("___ENTERING SETUP___");

  pinMode(TEENSY_LED, HIGH);

  for (int i = 0; i < NUM_ZONES; i++) {
    Serial.print("Init Zone #"); Serial.println(i);
    pinMode(PINS[i], LOW);
    STRIPS[i]->begin();
    STRIPS[i]->setBrightness(brightness_value);
    ZONES[i]->setZoneBrightness(brightness_value);
  }

  SetAllZonesToBlack();
  showAllZones();

  choix = 0;
  Serial.println("___END SETUP___");
}

/*********************************************************************************
   BOUCLE
 *********************************************************************************/
void loop() {

  board_blinking(500);

  // choix du scénario
  switch (choix) {
    case 0 : // Scénario de test des zones
      Serial.println("Scénario 0");
      for (int i = 0; i < NUM_ZONES; i++) {
        ZONES[i]->setZoneColor(255, 255, 255);
      }
      break;
    case 1 :
      Serial.println("Scénario 1");
      for (int i = 0; i < NUM_ZONES; i++) {
        ZONES[i]->setZoneColor(175, 60, 190);
      }
      break;
    default :
      choix = 0;
  }
  showAllZones();
}


/*
           for ( int i = 0; i < NUM_ZONES; i++) {
            for (int p = 0; p < STRIPS[i]->numPixels(); p++) {
              if (p % 6 == 0 ) {
                Serial.print("Strip #"); Serial.print(i);
                Serial.print(", Led #"); Serial.println(p);
                STRIPS[i]->setPixelColor(p, 255, 255, 255);
              } else {
                STRIPS[i]->setPixelColor(p, 0, 0, 0);
              }
            }
            //}
          }

 */
/*****************************************************************
   Binking function for onboard LED
 *****************************************************************/
void board_blinking(int freq) {
  int myMillis = millis();
  myMillis %= 2 * freq;
  if (myMillis >= freq) {
    digitalWrite(TEENSY_LED, HIGH);
  } else {
    digitalWrite(TEENSY_LED, LOW);
  }
}

/*****************************************************************
   Setting all zones to back.
 *****************************************************************/
void SetAllZonesToBlack() {
  // Tout à noir
  for (int i = 0; i < NUM_ZONES; i++) {
    ZONES[i]->setZoneColor(0, 0, 0);
  }
}

/*****************************************************************
  Showing all zones
 *****************************************************************/
void showAllZones() {
  // Néopixel
  for (int i = 0; i < NUM_ZONES; i++) {
    ZONES[i]->show();
  }
}


