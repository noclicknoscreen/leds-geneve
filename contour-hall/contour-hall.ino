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
#define NUM_ZONES 7

// on board Led
#define TEENSY_LED 13
// Signaux de pilotage des zones
# define PIN_ZONE_A 3
# define PIN_ZONE_B 4
# define PIN_ZONE_C 5
# define PIN_ZONE_D 6
# define PIN_ZONE_E 9
# define PIN_ZONE_F 10
# define PIN_ZONE_G 22  // 

// Definir les 6 zones a eclairer
// Valeurs réelles
# define NUM_LEDS_A 30
# define NUM_LEDS_B 58
# define NUM_LEDS_C 29 //58
# define NUM_LEDS_D 29 //58
# define NUM_LEDS_E 39
# define NUM_LEDS_F 49
# define NUM_LEDS_G 57

const int PINS[NUM_ZONES] = {PIN_ZONE_A, PIN_ZONE_B, PIN_ZONE_C, PIN_ZONE_D, PIN_ZONE_E, PIN_ZONE_F, PIN_ZONE_G};

Adafruit_NeoPixel pixels_A = Adafruit_NeoPixel(NUM_LEDS_A, PIN_ZONE_A, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel pixels_B = Adafruit_NeoPixel(NUM_LEDS_B, PIN_ZONE_B, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel pixels_C = Adafruit_NeoPixel(NUM_LEDS_C, PIN_ZONE_C, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel pixels_D = Adafruit_NeoPixel(NUM_LEDS_D, PIN_ZONE_D, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel pixels_E = Adafruit_NeoPixel(NUM_LEDS_E, PIN_ZONE_E, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel pixels_F = Adafruit_NeoPixel(NUM_LEDS_F, PIN_ZONE_F, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel pixels_G = Adafruit_NeoPixel(NUM_LEDS_G, PIN_ZONE_G, NEO_GRB + NEO_KHZ800);

Zone zoneA = Zone(&pixels_A, 1, NUM_LEDS_A);
Zone zoneB = Zone(&pixels_B, 1, NUM_LEDS_B);
Zone zoneC = Zone(&pixels_C, 1, NUM_LEDS_C);
Zone zoneD = Zone(&pixels_D, 1, NUM_LEDS_D);
Zone zoneE = Zone(&pixels_E, 1, NUM_LEDS_E);
Zone zoneF = Zone(&pixels_F, 1, NUM_LEDS_F);
Zone zoneG = Zone(&pixels_G, 1, NUM_LEDS_G);

Zone * ZONES[NUM_ZONES] = {&zoneA, &zoneB, &zoneC, &zoneD, &zoneE, &zoneF, &zoneG};

Adafruit_NeoPixel * STRIPS[NUM_ZONES] = {&pixels_A, &pixels_B, &pixels_C, &pixels_D, &pixels_E, &pixels_F, &pixels_G};

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


