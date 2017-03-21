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
#define NUM_ZONES 6
#define NUM_STRIPS 6

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

// Definir les 6 zones a eclairer
// Valeurs réelles

# define NUM_LEDS_A 82 // 90  // 30x2 + 15x2
# define NUM_LEDS_B 70 //76  // 23x2 + 15x2
# define NUM_LEDS_C 99 // 106 // 38x2 + 15x2
# define NUM_LEDS_D 86 //90  // 30x2 + 15x2
# define NUM_LEDS_E 88  // 29x2 + 15x2
# define NUM_LEDS_F 58 // 88  // 29x2 + 15x2

/* Valeurs pour la maquette
  # define NUM_LEDS_A 22
  # define NUM_LEDS_B 18
  # define NUM_LEDS_C 26
  # define NUM_LEDS_D 22
  # define NUM_LEDS_E 22
  # define NUM_LEDS_F 21
*/

Adafruit_NeoPixel pixels_A = Adafruit_NeoPixel(NUM_LEDS_A, PIN_ZONE_A, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel pixels_B = Adafruit_NeoPixel(NUM_LEDS_B, PIN_ZONE_B, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel pixels_C = Adafruit_NeoPixel(NUM_LEDS_C, PIN_ZONE_C, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel pixels_D = Adafruit_NeoPixel(NUM_LEDS_D, PIN_ZONE_D, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel pixels_E = Adafruit_NeoPixel(NUM_LEDS_E, PIN_ZONE_E, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel pixels_F = Adafruit_NeoPixel(NUM_LEDS_F, PIN_ZONE_F, NEO_GRB + NEO_KHZ800);

Zone zoneA = Zone(&pixels_A, 1, NUM_LEDS_A);
Zone zoneB = Zone(&pixels_B, 1, NUM_LEDS_B);
Zone zoneC = Zone(&pixels_C, 1, NUM_LEDS_C);
Zone zoneD = Zone(&pixels_D, 1, NUM_LEDS_D);
Zone zoneE = Zone(&pixels_E, 1, NUM_LEDS_E);
Zone zoneF = Zone(&pixels_F, 1, NUM_LEDS_F);

const int PINS[NUM_ZONES] = {PIN_ZONE_A, PIN_ZONE_B, PIN_ZONE_C, PIN_ZONE_D, PIN_ZONE_E, PIN_ZONE_F};

Zone * ZONES[NUM_ZONES] = {&zoneA, &zoneB, &zoneC, &zoneD, &zoneE, &zoneF};

//Adafruit_NeoPixel * STRIPS[NUM_ZONES] = {&pixels_A, &pixels_B, &pixels_C, &pixels_D, &pixels_E, &pixels_F};

int brightness_value = 255; // Between 0 and 100 %

// global index for a Volume
int k = 0;
// Couleur
int red = 255;
int green = 255;
int blue = 255;

String stripOrZone = "ZONE";
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

  for (int i = 0; i < NUM_STRIPS; i++) {
    Serial.print("Init Zone #"); Serial.println(i);
    pinMode(PINS[i], LOW);

    STRIPS[i]->begin();
    STRIPS[i]->setBrightness(brightness_value);
    setStripColor(STRIPS[i], 0, 0, 0);

    ZONES[i]->setZoneBrightness(brightness_value);
    ZONES[i]->setZoneColor(0, 0, 0);

    // Faire la somme de tous les pixels
    totalNumLeds += STRIPS[i]->numPixels();

  }

  // onBoard Lighting
  pinMode(TEENSY_LED, HIGH);

  // Pin de pilotage des scénarii
  pinMode(PIN_SC1, INPUT_PULLUP); // dry contact
  pinMode(PIN_SC2, INPUT_PULLUP); // dry contact
  pinMode(PIN_SC3, INPUT_PULLUP); // dry contact
  pinMode(PIN_SC4, INPUT_PULLUP); // dry contact

  showAllZones();

  // Init some random values
  k = random(0, NUM_ZONES);
  red = random(0, 255);
  green = random(0, 255);
  blue = random(0, 255);

  choix = 5;
  firstTime = true;
  holdTime = 1000;
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
      break;
    case 1 :
      if (firstTime) {
        Serial.println("Scénario #1, some fixed white colors.");
        firstTime = propagate_color(5000, 255, 255, 255);
        stripOrZone = "ZONE";
      } else {
        zoneA.setZoneColor(255, 255, 255);
        zoneB.setZoneColor(175, 60, 190);
        zoneC.setZoneColor(100, 107, 189);
        zoneD.setZoneColor(175, 82, 90);
        zoneE.setZoneColor(255, 255, 255);
        zoneF.setZoneColor(175, 60, 190);
      }
      break;
    case 2 :
      /*
         All color wipes
      */
      {
        // init for the first time
        if (firstTime) {
          Serial.println("Scénario #2, random colors in random zones, with a randomized hold time.");
          holdTime = 1000;
          stripOrZone = "ZONE";
          red = random(0, 255);
          green = random(0, 255);
          blue = random(0, 255);
          firstTime = propagate_color(5000, red, green, blue);
        }

        if (elapsedTime > holdTime) {
          k = random(0, NUM_ZONES);
          red = random(0, 255);
          green = random(0, 255);
          blue = random(0, 255);
          colorWipeZone(k, red, green, blue);
          elapsedTime -= holdTime;
          holdTime = random(1000, 10000);
          Serial.print("hoding time :"); Serial.print(holdTime / 1000, DEC); Serial.println(" seconds");
        }
        break;
      }
    case 3 :
      {
        // init for the first time
        if (firstTime) {
          Serial.println("Scénario #3, random green hues in random zones, with a randomized hild time.");
          holdTime = 1000;
          stripOrZone = "ZONE";
          red = random(0, 255);
          green = random(230, 255);
          blue = random(0, 100);
          firstTime = propagate_color(5000, red, green, blue);
        }

        if (elapsedTime > holdTime) {
          k = random(0, NUM_ZONES);
          red = random(0, 255);
          green = random(230, 255);
          blue = random(0, 100);
          colorWipeZone(k, red, green, blue);
          elapsedTime -= holdTime;
          holdTime = random(1000, 10000);
          Serial.print("hoding time :"); Serial.print(holdTime / 1000, DEC); Serial.println(" seconds");
        }
        break;
      }
    case 4 :
      {
        // init for the first time
        if (firstTime) {
          Serial.println("Scénario #4, Rainbow.");
          holdTime = 1000;
          stripOrZone = "STRIP";
          firstTime = false;
        } else {
          constrainedRainbow(25, 120, 6000.0f);
        }
        break;
      }
    case 5 :
      {
        if (firstTime) {
          Serial.println("Scénario #5, All the lights to black.");
          firstTime = false;
          // All the light OFF
          for (int i = 0; i < NUM_ZONES; i++) {
            ZONES[i]->setZoneColor(0, 0, 0);
            stripOrZone = "ZONE";
          }
          for (int i = 0; i < NUM_STRIPS; i++) {
            SetAllStripsToColor(0, 0, 0);
            stripOrZone = "STRIP";
          }
        }
        break;
      }
    default :
      choix = 1;
      break;
  }
  if (stripOrZone == "ZONE") {
    showAllZones();
  } else {
    showAllStrips();
  }

  // Read the 4 inputs to decide what to do
  read_choice();

}

void colorWipeZone(int zoneNum, int r, int g, int b) {
  Serial.print("Volume #"); Serial.println(zoneNum);
  Serial.print(r); Serial.print(":"); Serial.print(g); Serial.print(":"); Serial.println(b);
  ZONES[zoneNum]->setZoneColor(r, g, b);
}

/*
   Propagate color in all zones
*/
boolean propagate_color(int freq, int r, int g, int b) {
  float perCent = fmod(millis(), freq) / freq;
  perCent *= 100.0f;

  float zoneA_r, zoneA_g, zoneA_b,
        zoneB_r, zoneB_g, zoneB_b,
        zoneC_r, zoneC_g, zoneC_b,
        zoneD_r, zoneD_g, zoneD_b,
        zoneE_r, zoneE_g, zoneE_b,
        zoneF_r, zoneF_g, zoneF_b;

  zoneA_r = clampMap(perCent,  0,  16,   0, r);
  zoneB_r = clampMap(perCent,  16,  32,   0, r);
  zoneC_r = clampMap(perCent,  32, 48,   0, r);
  zoneD_r = clampMap(perCent,  48,  64,   0, r);
  zoneE_r = clampMap(perCent,  64,  78,   0, r);
  zoneF_r = clampMap(perCent,  78,  96,   0, r);
  zoneA_g = clampMap(perCent,  0,  16,   0, g);
  zoneB_g = clampMap(perCent,  16,  32,   0, g);
  zoneC_g = clampMap(perCent,  32, 48,   0, g);
  zoneD_g = clampMap(perCent,  48,  64,   0, g);
  zoneE_g = clampMap(perCent,  64,  78,   0, g);
  zoneF_g = clampMap(perCent,  78,  96,   0, g);
  zoneA_b = clampMap(perCent,  0,  16,   0, b);
  zoneB_b = clampMap(perCent,  16,  32,   0, b);
  zoneC_b = clampMap(perCent,  32, 48,   0, b);
  zoneD_b = clampMap(perCent,  48,  64,   0, b);
  zoneE_b = clampMap(perCent,  64,  78,   0, b);
  zoneF_b = clampMap(perCent,  78,  96,   0, b);

  zoneA.setZoneColor(zoneA_r, zoneA_g, zoneA_b);
  zoneB.setZoneColor(zoneB_r, zoneB_g, zoneB_b);
  zoneC.setZoneColor(zoneC_r, zoneC_g, zoneC_b);
  zoneD.setZoneColor(zoneD_r, zoneD_g, zoneD_b);
  zoneE.setZoneColor(zoneE_r, zoneE_g, zoneE_b);
  zoneF.setZoneColor(zoneF_r, zoneF_g, zoneF_b);

  if (zoneF_b >= b) {
    return true;
  }
  return false;
}

/*****************************************************************
   Framwork
 *****************************************************************/
void showAllZones() {
  for (int i = 0; i < NUM_ZONES; i++) {
    ZONES[i]->show();
  }
}


