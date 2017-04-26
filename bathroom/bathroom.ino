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
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif

// Nombre de zones
#define NUM_STRIPS 8
#define NUM_ZONES 8

// Signaux de pilotage des scénarii
#define PIN_SC1 7
#define PIN_SC2 8
#define PIN_SC3 A0
#define PIN_SC4 A1

#include <ncns-leds-lib.h>

// Signaux de pilotage des zones
# define PIN_1 3
# define PIN_2 4
# define PIN_3 5
# define PIN_4 6
# define PIN_5 9
# define PIN_6 10
# define PIN_7 21
# define PIN_8 22

// Definir les 6 zones a eclairer
// Valeurs réelles
#define NUM_LEDS_V01 43 // 43 + 
#define NUM_LEDS_V02 60 // + 51
#define NUM_LEDS_V03 60 // 32 +
#define NUM_LEDS_V04 79 // 34 + 38 + 7 OK
#define NUM_LEDS_V05 57 // 5 + 52   OK
#define NUM_LEDS_V06 60 // 23 + 
#define NUM_LEDS_V07 60 // 41 + 
#define NUM_LEDS_V08 64 // 57 + 7   OK

Adafruit_NeoPixel leds1 = Adafruit_NeoPixel( NUM_LEDS_V01, PIN_1,  NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel leds2 = Adafruit_NeoPixel( NUM_LEDS_V02, PIN_2,  NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel leds3 = Adafruit_NeoPixel( NUM_LEDS_V03, PIN_3,  NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel leds4 = Adafruit_NeoPixel( NUM_LEDS_V04, PIN_4,  NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel leds5 = Adafruit_NeoPixel( NUM_LEDS_V05, PIN_5,  NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel leds6 = Adafruit_NeoPixel( NUM_LEDS_V06, PIN_6,  NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel leds7 = Adafruit_NeoPixel( NUM_LEDS_V07, PIN_7,  NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel leds8 = Adafruit_NeoPixel( NUM_LEDS_V08, PIN_8,  NEO_GRB + NEO_KHZ800);

// Zone zoneA = Zone(&leds1, 1, NUM_LEDS_V01);
// Zone zoneB = Zone(&leds2, 1, NUM_LEDS_V02);
// Zone zoneC = Zone(&leds3, 1, NUM_LEDS_V03);
// Zone zoneD = Zone(&leds4, 1, NUM_LEDS_V04);
// Zone zoneE = Zone(&leds5, 1, NUM_LEDS_V05);
// Zone zoneF = Zone(&leds6, 1, NUM_LEDS_V06);
// Zone zoneG = Zone(&leds7, 1, NUM_LEDS_V07);
// Zone zoneH = Zone(&leds8, 1, NUM_LEDS_V08);


// Zone * ZONES[NUM_STRIPS] = { &zoneA, &zoneB, &zoneC, &zoneD, &zoneE, &zoneF, &zoneG, &zoneH };

const int PINS[NUM_STRIPS] = {PIN_1, PIN_2, PIN_3, PIN_4, PIN_5, PIN_6, PIN_7, PIN_8};
const int NUM_LEDS[] = {
    NUM_LEDS_V01, NUM_LEDS_V02, NUM_LEDS_V03, NUM_LEDS_V04, 
    NUM_LEDS_V05, NUM_LEDS_V06, NUM_LEDS_V07, NUM_LEDS_V08
};

//Adafruit_NeoPixel * STRIPS[NUM_STRIPS] = {&leds1, &leds2, &leds3, &leds4, &leds5, &leds6, &leds7, &leds8};

int brightness_value = 255;
int refreshMode = ZONE_MODE;

// global index for a Volume
int k = 0;
// Couleur
int red = 255;
int green = 255;
int blue = 255;

int intervall = 0;
/*********************************************************************************
   Setup
 *********************************************************************************/
void setup()
{
  Serial.begin(9600);

  STRIPS[0] = &leds1;
  STRIPS[1] = &leds2;
  STRIPS[2] = &leds3;
  STRIPS[3] = &leds4;
  STRIPS[4] = &leds5;
  STRIPS[5] = &leds6;
  STRIPS[6] = &leds7;
  STRIPS[7] = &leds8;

  // Pin de pilotage des scénarii
  pinMode(PIN_SC1, INPUT_PULLUP); // dry contact
  pinMode(PIN_SC2, INPUT_PULLUP); // dry contact
  pinMode(PIN_SC3, INPUT_PULLUP); // dry contact
  pinMode(PIN_SC4, INPUT_PULLUP); // dry contact

  for (int i = 0; i < NUM_STRIPS; i++) {
    Serial.print("Init Zone #"); Serial.println(i);
    pinMode(PINS[i], LOW);
    STRIPS[i]->begin();
    STRIPS[i]->setBrightness(brightness_value);
    setStripColor(STRIPS[i], 0, 0, 0);

    // Faire la somme de tous les pixels
    totalNumLeds += STRIPS[i]->numPixels();

  }

  choix = 5;
}

/*********************************************************************************
   BOUCLE
 *********************************************************************************/
void loop() {
  board_blinking(500);

  // choix du scénario
  switch (choix) {
    case 1 :
      /*
         Scénario statique sans trop de changement
      */
      if (firstTime) {
        Serial.println("Scénario 1 : Blanc allumage progressif");
        firstTime = false;
        holdTime = 100;
        intervall = 10;
        refreshMode = STRIP_MODE;
      }
      if (elapsedTime > holdTime && intervall > 0) {
        setColorOneLedEvery(intervall, 100, 160, 190);
        elapsedTime -= holdTime;
        intervall--;
      }
      break;
    case 2 :
      /*
         Du bord vers le centre, puis du centre vers le bord, avec une autre couleur
         On rattaque ensuite avec la couleur avec laquelle on a fini
      */
      {
        if (firstTime) {
          red = random(0, 255);
          green = random(230, 255);
          blue = random(0, 100);
          firstTime = false;
          refreshMode = STRIP_MODE;
          holdTime = 4000;
        }

        if (elapsedTime > holdTime) {
          for (int i = 0; i < int(NUM_STRIPS / 2); i++) {
            setStripColor(STRIPS[i], red, green, blue );
            setStripColor(STRIPS[NUM_STRIPS - i], red, green, blue );
            delay(50);
          }
          red = random(0, 255);
          green = random(230, 255);
          blue = random(0, 100);
          for (int i = int(NUM_STRIPS / 2); i >= 0; i--) {
            setStripColor(STRIPS[i], red, green, blue );
            setStripColor(STRIPS[NUM_STRIPS - i], red, green, blue );
            delay(50);
          }
          elapsedTime -= holdTime;
          holdTime = random(10000, 20000);
        }

        break;
      }
    case 3 :
      {
        // init for the first time
        if (firstTime) {
          Serial.println("Scénario #3, random green hues in random zones, with a randomized hild time.");
          holdTime = 1000;
          refreshMode = ZONE_MODE;
          red = random(0, 255);
          green = random(230, 255);
          blue = random(0, 100);
          firstTime = propagate_color(5000, red, green, blue);
        }

        if (elapsedTime > holdTime) {
          k = random(0, NUM_STRIPS);
          red = random(0, 255);
          green = random(230, 255);
          blue = random(0, 100);
          setStripColor(STRIPS[k], red, green, blue);
          elapsedTime -= holdTime;
          holdTime = random(1000, 10000);
          Serial.print("hoding time :"); Serial.print(holdTime / 1000, DEC); Serial.println(" seconds");
        }
        break;
      }
    case 4 :
      if (firstTime) {
        Serial.println("Scénario 4 : Constrained Rainbow 100 to 190");
        firstTime = false;
        refreshMode = STRIP_MODE;
      } else {
        constrainedRainbow(100, 190, 10000.0f);
      }
    case 5 :
      if (firstTime) {
        Serial.println("Scénario #5, All the lights to black.");
        firstTime = false;
        // All the light OFF
        refreshMode = STRIP_MODE;
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


// void colorWipeZone(int zoneNum, int r, int g, int b) {
//   Serial.print("Volume #"); Serial.println(zoneNum);
//   Serial.print(r); Serial.print(":"); Serial.print(g); Serial.print(":"); Serial.println(b);
//   ZONES[zoneNum]setStripColor(r, g, b);
// }

/*
   Propagate color in all zones
*/
boolean propagate_color(int freq, int r, int g, int b) {
  float perCent = fmod(millis(), freq) / freq;
  perCent *= 100.0f;

  float strip1_r, strip1_g, strip1_b,
        strip2_r, strip2_g, strip2_b,
        strip3_r, strip3_g, strip3_b,
        strip4_r, strip4_g, strip4_b,
        strip5_r, strip5_g, strip5_b,
        strip6_r, strip6_g, strip6_b,
        strip7_r, strip7_g, strip7_b,
        strip8_r, strip8_g, strip8_b;

  strip1_r = clampMap(perCent,   0,   12,   0, r);
  strip2_r = clampMap(perCent,  13,   24,   0, r);
  strip3_r = clampMap(perCent,  25,   37,   0, r);
  strip4_r = clampMap(perCent,  38,   50,   0, r);
  strip5_r = clampMap(perCent,  51,   62,   0, r);
  strip6_r = clampMap(perCent,  63,   75,   0, r);
  strip7_r = clampMap(perCent,  76,   87,   0, r);
  strip8_r = clampMap(perCent,  88,  100,   0, r);
  strip1_g = clampMap(perCent,   0,   12,   0, g);
  strip2_g = clampMap(perCent,  13,   24,   0, g);
  strip3_g = clampMap(perCent,  25,   37,   0, g);
  strip4_g = clampMap(perCent,  38,   50,   0, g);
  strip5_g = clampMap(perCent,  51,   62,   0, g);
  strip6_g = clampMap(perCent,  63,   75,   0, g);
  strip7_g = clampMap(perCent,  76,   87,   0, g);
  strip8_g = clampMap(perCent,  88,  100,   0, g);
  strip1_b = clampMap(perCent,   0,   12,   0, b);
  strip2_b = clampMap(perCent,  13,   24,   0, b);
  strip3_b = clampMap(perCent,  25,   37,   0, b);
  strip4_b = clampMap(perCent,  38,   50,   0, b);
  strip5_b = clampMap(perCent,  51,   62,   0, b);
  strip6_b = clampMap(perCent,  63,   75,   0, b);
  strip7_b = clampMap(perCent,  76,   87,   0, b);
  strip8_b = clampMap(perCent,  88,  100,   0, b);

  setStripColor ( STRIPS[1], strip1_r, strip1_g, strip1_b);
  setStripColor ( STRIPS[2], strip2_r, strip2_g, strip2_b);
  setStripColor ( STRIPS[3], strip3_r, strip3_g, strip3_b);
  setStripColor ( STRIPS[4], strip4_r, strip4_g, strip4_b);
  setStripColor ( STRIPS[5], strip5_r, strip5_g, strip5_b);
  setStripColor ( STRIPS[6], strip6_r, strip6_g, strip6_b);
  setStripColor ( STRIPS[7], strip7_r, strip7_g, strip7_b);
  setStripColor ( STRIPS[8], strip8_r, strip8_g, strip8_b);

  if (strip8_b >= b) {
    return true;
  }
  return false;
}
/*****************************************************************
   Setting all zones to back.
 *****************************************************************/
void SetAllZonesToColor(int r, int g, int b) {
  // Tout à noir
  for (int i = 0; i < NUM_STRIPS; i++) {
    setStripColor(STRIPS[i], r, g, b);
  }
}

/*****************************************************************
  Showing all zones
 *****************************************************************/
void showAllZones() {
  // Néopixel
  for (int i = 0; i < NUM_STRIPS; i++) {
    STRIPS[i]->show();
  }
}

