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

// Mode TEST : Décommenter cette ligne
//#define TEST_MODE 0
// 0 - Test des scénariis un par un (1-5)
// 2 = Test des Strips
//---------------------------------------
#define TEST_SCENARIO 4

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
// Valeurs selon fiche de prod
#define NUM_LEDS_V01 76 // 43 + 33
#define NUM_LEDS_V02 70 // 19 + 51
#define NUM_LEDS_V03 67 // 32 + 35
#define NUM_LEDS_V04 79 // 34 + 38 + 7
#define NUM_LEDS_V05 57 // 5 + 52
#define NUM_LEDS_V06 66 // 23 + 43
#define NUM_LEDS_V07 59 // 41 + 18
#define NUM_LEDS_V08 96 // 57 + 7 + 32

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


/* ---------------------------
   Color informations
   0 = RED
   50 = LEMON YELLOW
   100 = TURQUOISE GREEN
   150 = PURPLE BLUE
   200 = PINK PURPLE
   255 = RED
*/

int brightness_value = 255;
int refreshMode = ZONE_MODE;

// global index for a Volume
int k = 0;
// Couleur
int red = 255;
int green = 255;
int blue = 255;

/* ---------------------------------------
  Scénario 2
  ------------------------------------------*/
uint32_t newColor = 0;
#define SC2_LOW   50
#define SC2_HIGH  150
#define HOLDTIME  5000.0f
/* ---------------------------------------
  Scénario 3
  ------------------------------------------*/
#define SC3_COLOR 150 // Pink
#define SC3_WHITE 255
/* ---------------------------------------
  Scénario 4
  ------------------------------------------*/
#define SC4_LOW     130
#define SC4_HIGH    255
#define SC4_PERIOD  10000.0f

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

  // En test, on écrase la valeur
#ifdef TEST_MODE
  choix = TEST_SCENARIO;
#endif

}

/*********************************************************************************
   BOUCLE
 *********************************************************************************/
void loop() {
  board_blinking(500);

  // Mode TEST
  // Tester toutes les zones à chaque appuie sur la touche 'T'
#ifdef TEST_MODE
  if (TEST_MODE != 0) {
    testerZonesEtStrips();
  }
#endif

#ifdef TEST_MODE
  if (TEST_MODE == 0) {
#endif

    // LOG ----------
    //logKnxInputs();

    // choix du scénario
    switch (choix) {
      case 1 :
        /*
           Scénario statique sans trop de changement
        */
        if (firstTime) {
          Serial.println("Scénario 1 : Progressive white lightening");
          firstTime = false;
          holdTime = 10;
          intervall = 2;
          refreshMode = STRIP_MODE;
        }
        if (elapsedTime > holdTime && intervall > 0) {
          setColorOneLedEvery(intervall, 100, 160, 190);
          elapsedTime -= holdTime;
          intervall--;
        }
        break;
      case 2 :
        {

          // init for the first time
          if (firstTime) {
            Serial.println("Scénario #2, random colors in random zones, with a randomized hold time.");

            refreshMode = STRIP_MODE;
            constrainedRainbow(SC2_LOW, SC2_HIGH, 10000.0f);

            firstTime = false;
          }

          colorWipeStripBathroom(STRIPS[k], HOLDTIME, newColor);

          if (elapsedTime > HOLDTIME) {
            k = random(0, NUM_STRIPS);

            newColor = Wheel(random(SC2_LOW, SC2_HIGH));
            elapsedTime -= HOLDTIME;

            Serial.print("Strip #"); Serial.println(k);
            Serial.print("Color : ");
            Serial.print(redFrom(newColor)); Serial.print(":"); Serial.print(greenFrom(newColor)); Serial.print(":"); Serial.println(blueFrom(newColor));

          }


          /*          Serial.print("Elapsed time = "); Serial.print(elapsedTime);
                    Serial.print(" HOLDTIME = "); Serial.print(HOLDTIME);
                    Serial.print(" wipeFinished = "); Serial.print(wipeFinished);
                    Serial.println();
          */


          break;
        }
      case 3 :
        if (firstTime) {
          Serial.println("Scénario #3 : Sinusoidal brightness on a led train");
          firstTime = false;
          refreshMode = STRIP_MODE;
        } else {
          sinusoidalTheaterChaseBathroom(60, 20000.0f, SC3_COLOR, SC3_WHITE);
        }
        break;
      case 4 :
        if (firstTime) {
          Serial.println("Scénario 4 : Constrained Rainbow LOW to HIGH");
          firstTime = false;
          refreshMode = STRIP_MODE;
        } else {
          constrainedRainbow(SC4_LOW, SC4_HIGH, SC4_PERIOD);
        }
        break;
      case 5 :
        if (firstTime) {
          Serial.println("Scénario #5, All the lights to black.");
          firstTime = false;
          // All the light OFF
          refreshMode = STRIP_MODE;
        }

        SetAllStripsToColor(0, 0, 0);
        break;
      default :
        choix = 1;
    }
    showAllStrips();

#ifdef TEST_MODE
  } // end if TEST_MODE == 0
#endif

#ifndef TEST_MODE
  // Reading choice
  read_choice();
#endif

}

