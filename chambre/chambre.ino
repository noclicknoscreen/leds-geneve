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
//#include <Zones.h>
#ifdef __AVR__
#include <avr/power.h>
#endif

// Mode TEST : Décommenter cette ligne
//#define TEST_MODE 2
// 0 - Test des scénariis un par un (1-5)
// 2 = Test des Strips
//---------------------------------------

// Nombre de zones
#define NUM_ZONES 24
#define NUM_STRIPS 8

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
#define NUM_LEDS_A 15               // OK
#define NUM_LEDS_B 44               // OK
#define NUM_LEDS_C 43               // OK
#define NUM_LEDS_D 44 // 28 + 16    // OK
#define NUM_LEDS_E 33 // 27 + 6     // OK
#define NUM_LEDS_F 9  // 2 + 7      // OK
#define NUM_LEDS_G 19               // OK
#define NUM_LEDS_H 9                // OK 
#define NUM_LEDS_I 8                // OK 
#define NUM_LEDS_J 5 // 3 + 2       // OK
#define NUM_LEDS_K 8 //             // OK
#define NUM_LEDS_L 6 // 2 + 4       // OK
#define NUM_LEDS_M 5 // 2 + 3       // OK 
#define NUM_LEDS_N 9 // 4 + 5       // OK
#define NUM_LEDS_O 8                // OK
#define NUM_LEDS_P 6 // 3 + 3       // OK
#define NUM_LEDS_Q 20               // OK
#define NUM_LEDS_R 16 // 8 + 8      // OK
#define NUM_LEDS_S 26               // OK
#define NUM_LEDS_T 29               // OK 
#define NUM_LEDS_U 40 // 19 + 21    // OK
#define NUM_LEDS_V 44               // OK
#define NUM_LEDS_W 41 // 30 + 11    // OK
#define NUM_LEDS_X 15               // OK

Adafruit_NeoPixel leds1 = Adafruit_NeoPixel( NUM_LEDS_A, PIN_1 ,  NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel leds2 = Adafruit_NeoPixel( NUM_LEDS_B + NUM_LEDS_C, PIN_2 ,  NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel leds3 = Adafruit_NeoPixel( NUM_LEDS_D + NUM_LEDS_E, PIN_3 ,  NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel leds4 = Adafruit_NeoPixel( NUM_LEDS_F + NUM_LEDS_G + NUM_LEDS_H + NUM_LEDS_I + NUM_LEDS_J + NUM_LEDS_K + NUM_LEDS_L + NUM_LEDS_M + NUM_LEDS_N + NUM_LEDS_O + NUM_LEDS_P + NUM_LEDS_Q + NUM_LEDS_R, PIN_4 ,  NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel leds5 = Adafruit_NeoPixel( NUM_LEDS_S + NUM_LEDS_T, PIN_5 ,  NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel leds6 = Adafruit_NeoPixel( NUM_LEDS_U, PIN_6 ,  NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel leds7 = Adafruit_NeoPixel( NUM_LEDS_V + NUM_LEDS_W, PIN_7 ,  NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel leds8 = Adafruit_NeoPixel( NUM_LEDS_X, PIN_8 ,  NEO_GRB + NEO_KHZ800);

const int PINS[NUM_STRIPS] = {PIN_1, PIN_2, PIN_3, PIN_4, PIN_5, PIN_6, PIN_7, PIN_8};
const int NUM_LEDS[] = {
  NUM_LEDS_A, NUM_LEDS_B, NUM_LEDS_C, NUM_LEDS_D,
  NUM_LEDS_E, NUM_LEDS_F, NUM_LEDS_G, NUM_LEDS_H,
  NUM_LEDS_I, NUM_LEDS_J, NUM_LEDS_K, NUM_LEDS_L,
  NUM_LEDS_M, NUM_LEDS_N, NUM_LEDS_O, NUM_LEDS_P,
  NUM_LEDS_Q, NUM_LEDS_R, NUM_LEDS_S, NUM_LEDS_T,
  NUM_LEDS_U, NUM_LEDS_V, NUM_LEDS_W, NUM_LEDS_X
};

//Adafruit_NeoPixel * STRIPS[NUM_ZONES] = {&leds1, &leds2, &leds3, &leds4, &leds5, &leds6, &leds7, &leds8};

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
          int wipeFinished = true;

          // init for the first time
          if (firstTime) {
            Serial.println("Scénario #2, random colors in random zones, with a randomized hold time.");
            holdTime = 5000;
            refreshMode = STRIP_MODE;
            constrainedRainbow(10, 50, 10000.0f);
            firstTime = false;
          }
          if (wipeFinished && elapsedTime > holdTime) {
            k = random(0, NUM_STRIPS);
            red = random(0, 255);
            green = random(0, 255);
            blue = random(0, 255);
            elapsedTime -= holdTime;
            Serial.print("Strip #"); Serial.println(k);
            Serial.print(red); Serial.print(":"); Serial.print(green); Serial.print(":"); Serial.println(blue);
          }
          wipeFinished = colorWipeStrip(STRIPS[k], holdTime, red, green, blue);
          break;
        }
      case 3 :
        if (firstTime) {
          Serial.println("Scénario #3 : Sinusoidal brightness on a led train");
          firstTime = false;
          refreshMode = STRIP_MODE;
          red = random(200, 230);
          green = random(180, 230);
          blue = random(0, 40);
        } else {
          sinusoidalTheaterChase(60, 20000.0f, red, green, blue);
        }
        break;
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
    showAllStrips();

#ifdef TEST_MODE
  } // end if TEST_MODE == 0
#endif

  // Reading choice
  read_choice();
}

