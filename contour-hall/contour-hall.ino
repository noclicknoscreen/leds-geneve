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
#define NUM_STRIPS 7

// Signaux de pilotage des scénarii
#define PIN_SC1 7
#define PIN_SC2 8
#define PIN_SC3 A0
#define PIN_SC4 A1

#include <ncns-leds-lib.h>

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

//Adafruit_NeoPixel * STRIPS[NUM_ZONES] = {&pixels_A, &pixels_B, &pixels_C, &pixels_D, &pixels_E, &pixels_F, &pixels_G};

int brightness_value = 255; // Between 0 and 100 %
int intervall = 0;

int refreshMode = ZONE_MODE;
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

  STRIPS[0] = &pixels_A;
  STRIPS[1] = &pixels_B;
  STRIPS[2] = &pixels_C;
  STRIPS[3] = &pixels_D;
  STRIPS[4] = &pixels_E;
  STRIPS[5] = &pixels_F;
  STRIPS[6] = &pixels_G;

  pinMode(TEENSY_LED, HIGH);

  // Pin de pilotage des scénarii
  pinMode(PIN_SC1, INPUT_PULLUP); // dry contact
  pinMode(PIN_SC2, INPUT_PULLUP); // dry contact
  pinMode(PIN_SC3, INPUT_PULLUP); // dry contact
  pinMode(PIN_SC4, INPUT_PULLUP); // dry contact

  for (int i = 0; i < NUM_ZONES; i++) {
    Serial.print("Init Zone #"); Serial.println(i);
    pinMode(PINS[i], LOW);
    STRIPS[i]->begin();
    STRIPS[i]->setBrightness(brightness_value);
    ZONES[i]->setZoneBrightness(brightness_value);
    // Faire la somme de tous les pixels
    totalNumLeds += STRIPS[i]->numPixels();
  }

  SetAllZonesToColor(0, 0, 0);
  showAllZones();

  choix = 5;
  Serial.println("___END SETUP___");
}

/*********************************************************************************
   BOUCLE
 *********************************************************************************/
void loop() {

  board_blinking(500);

  // choix du scénario
  switch (choix) {
    case 1 :
      if (firstTime) {
        Serial.println("Scénario 1 : Blanc violacé");
        firstTime = false;
        holdTime = 100;
        intervall = 10;
        refreshMode = STRIP_MODE;
      }
      if (elapsedTime > holdTime && intervall > 0) {
        setColorOneLedEvery(intervall, 175, 60, 190);
        elapsedTime -= holdTime;
        intervall--;
      }
      //SetAllZonesToColor(175, random(50, 65), 190);
      break;
    case 2 :
      if (firstTime) {
        Serial.println("Scénario 2 : Blanc pur");
        firstTime = false;
        refreshMode = ZONE_MODE;
      }
      SetAllZonesToColor(255, 255, 255);
      break;
    case 3 :
      if (firstTime) {
        Serial.println("Scénario 3 : Blanc tirant sur le vert");
        firstTime = false;
        refreshMode = ZONE_MODE;
      }
      SetAllZonesToColor(random(100, 150), 255, 230);
      break;
    case 4 :
      if (firstTime) {
        Serial.println("Scénario 4 : Blanc pur");
        firstTime = false;
        refreshMode = ZONE_MODE;
      }
      SetAllZonesToColor(255, 255, 255);
    case 5 :
      if (firstTime) {
        Serial.println("Scénario #5, All the lights to black.");
        firstTime = false;
        // All the light OFF
        SetAllZonesToColor(0, 0, 0);
        for (int i = 0; i < NUM_STRIPS; i++) {
          SetAllStripsToColor(0, 0, 0);
        }
      }
      break;
    default :
      choix = 1;
  }
  if (refreshMode == ZONE_MODE) {
    showAllZones();
  } else {
    showAllStrips();
  }

  // Reading choice
  read_choice();
}

/*****************************************************************
   Setting all zones to back.
 *****************************************************************/
void SetAllZonesToColor(int r, int g, int b) {
  // Tout à noir
  for (int i = 0; i < NUM_ZONES; i++) {
    ZONES[i]->setZoneColor(r, g, b);
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

